/**
 * @file main.c
 * @brief Main routine
 *
 * @section License
 *
 * Copyright (C) 2010-2014 Oryx Embedded. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded (www.oryx-embedded.com)
 * @version 1.5.0
 **/

//Dependencies
#include <stdlib.h>
#include "stm32f4xx.h"
#include "stm324xg_eval.h"
#include "stm324xg_eval_ioe.h"
#include "stm324xg_eval_lcd.h"
#include "lis302dl.h"
#include "os_port.h"
#include "core/tcp_ip_stack.h"
#include "drivers/stm32f4x7_eth.h"
#include "drivers/dp83848.h"
#include "dhcp/dhcp_client.h"
#include "ipv6/slaac.h"
#include "std_services/echo.h"
#include "std_services/discard.h"
#include "std_services/chargen.h"
#include "smtp/smtp_client.h"
#include "http/http_server.h"
#include "http/mime.h"
#include "yarrow.h"
#include "str.h"
#include "path.h"
#include "date_time.h"
#include "resource_manager.h"
#include "ext_int_driver.h"
#include "debug.h"

//Application configuration
#define APP_MAC_ADDR "00-AB-CD-EF-04-07"

#define APP_USE_DHCP ENABLED
#define APP_IPV4_HOST_ADDR "192.168.0.20"
#define APP_IPV4_SUBNET_MASK "255.255.255.0"
#define APP_IPV4_DEFAULT_GATEWAY "192.168.0.254"
#define APP_IPV4_PRIMARY_DNS "8.8.8.8"
#define APP_IPV4_SECONDARY_DNS "8.8.4.4"

#define APP_USE_SLAAC ENABLED
#define APP_IPV6_LINK_LOCAL_ADDR "fe80::407"
#define APP_IPV6_PREFIX "2001:db8::"
#define APP_IPV6_PREFIX_LENGTH 64
#define APP_IPV6_GLOBAL_ADDR "2001:db8::407"
#define APP_IPV6_ROUTER "fe80::1"
#define APP_IPV6_PRIMARY_DNS "2001:4860:4860::8888"
#define APP_IPV6_SECONDARY_DNS "2001:4860:4860::8844"

//Forward declaration of functions
HttpAccessStatus httpServerAuthCallback(HttpConnection *connection,
   const char_t *user, const char_t *uri);

error_t httpServerCgiCallback(HttpConnection *connection,
   const char_t *param);

error_t httpServerUriNotFoundCallback(HttpConnection *connection,
   const char_t *uri);

//Global variables
uint_t lcdLine = 0;
uint_t lcdColumn = 0;
uint_t adcValue = 0;
uint_t joystickState = 0;
int8_t ax = 0;
int8_t ay = 0;
int8_t az = 0;

DhcpClientSettings dhcpClientSettings;
DhcpClientCtx dhcpClientContext;
SlaacSettings slaacSettings;
SlaacContext slaacContext;
HttpServerSettings httpServerSettings;
HttpServerContext httpServerContext;
YarrowContext yarrowContext;
uint8_t seed[32];


/**
 * @brief Set cursor location
 * @param[in] line Line number
 * @param[in] column Column number
 **/

void lcdSetCursor(uint_t line, uint_t column)
{
   lcdLine = MIN(line, 10);
   lcdColumn = MIN(column, 20);
}


/**
 * @brief Write a character to the LCD display
 * @param[in] c Character to be written
 **/

void lcdPutChar(char_t c)
{
   if(c == '\r')
   {
      lcdColumn = 0;
   }
   else if(c == '\n')
   {
      lcdColumn = 0;
      lcdLine++;
   }
   else if(lcdLine < 10 && lcdColumn < 20)
   {
      //Display current character
      LCD_DisplayChar(lcdLine * 24, 319 - (lcdColumn * 16), c);

      //Advance the cursor position
      if(++lcdColumn >= 20)
      {
         lcdColumn = 0;
         lcdLine++;
      }
   }
}


/**
 * @brief ADC3 initialization
 **/

void adc3Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   ADC_CommonInitTypeDef ADC_CommonInitStructure;
   ADC_InitTypeDef ADC_InitStructure;

   //Enable peripheral clocks
   RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

   //Configure PF9 as an analog input
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
   GPIO_Init(GPIOF, &GPIO_InitStructure);

   //Common initialization
   ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
   ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
   ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
   ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
   ADC_CommonInit(&ADC_CommonInitStructure);

   //ADC3 initialization
   ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
   ADC_InitStructure.ADC_ScanConvMode = DISABLE;
   ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
   ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
   ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
   ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
   ADC_InitStructure.ADC_NbrOfConversion = 1;
   ADC_Init(ADC3, &ADC_InitStructure);

   //ADC3 regular channel7 configuration
   ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_3Cycles);

   //Enable ADC3
   ADC_Cmd(ADC3, ENABLE);
}


/**
 * @brief User task
 **/

void userTask(void *param)
{
   char_t buffer[40];

   //Point to the network interface
   NetInterface *interface = &netInterface[0];

   //Initialize LCD display
   lcdSetCursor(1, 0);
   printf("IPv4 Addr");
   lcdSetCursor(4, 0);
   printf("IPv6 Link-Local Addr");
   lcdSetCursor(7, 0);
   printf("IPv6 Global Addr");

   //Endless loop
   while(1)
   {
#if (IPV4_SUPPORT == ENABLED)
      //Display IPv4 host address
      lcdSetCursor(2, 0);
      printf("%-16s", ipv4AddrToString(interface->ipv4Config.addr, buffer));
#endif

#if (IPV6_SUPPORT == ENABLED)
      //Display IPv6 link-local address
      lcdSetCursor(5, 0);
      printf("%-40s", ipv6AddrToString(&interface->ipv6Config.linkLocalAddr, buffer));

      //Display IPv6 global address
      lcdSetCursor(8, 0);
      printf("%-40s", ipv6AddrToString(&interface->ipv6Config.globalAddr, buffer));
#endif

      //Start A/D conversion
      ADC_SoftwareStartConv(ADC3);
      //Wait until conversion completion
      while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET);
      //Get conversion result
      adcValue = ADC_GetConversionValue(ADC3);

      //Get joystick state
      joystickState = IOE_JoyStickGetState();

      //Retrieve acceleration along the 3 axis
      lis302dlGetAcc(&ax, &ay, &az);

      //Loop delay
      osDelayTask(500);
   }
}


/**
 * @brief LED blinking task
 **/

void blinkTask(void *parameters)
{
   //Endless loop
   while(1)
   {
      STM_EVAL_LEDOn(LED1);
      osDelayTask(100);
      STM_EVAL_LEDOff(LED1);
      osDelayTask(900);
   }
}


/**
 * @brief Main entry point
 * @return Unused value
 **/

int_t main(void)
{
   error_t error;
   uint_t i;
   uint32_t value;
   NetInterface *interface;
   OsTask *task;
   MacAddr macAddr;
#if (APP_USE_DHCP == DISABLED)
   Ipv4Addr ipv4Addr;
#endif
#if (APP_USE_SLAAC == DISABLED)
   Ipv6Addr ipv6Addr;
#endif

   //Initialize kernel
   osInitKernel();
   //Configure debug UART
   debugInit(115200);

   //Start-up message
   TRACE_INFO("\r\n");
   TRACE_INFO("**********************************\r\n");
   TRACE_INFO("*** CycloneTCP Web Server Demo ***\r\n");
   TRACE_INFO("**********************************\r\n");
   TRACE_INFO("Copyright: 2010-2014 Oryx Embedded\r\n");
   TRACE_INFO("Compiled: %s %s\r\n", __DATE__, __TIME__);
   TRACE_INFO("Target: STM32F407\r\n");
   TRACE_INFO("\r\n");

   //LED configuration
   STM_EVAL_LEDInit(LED1);
   STM_EVAL_LEDInit(LED2);
   STM_EVAL_LEDInit(LED3);
   STM_EVAL_LEDInit(LED4);

   //Clear LEDs
   STM_EVAL_LEDOff(LED1);
   STM_EVAL_LEDOff(LED2);
   STM_EVAL_LEDOff(LED3);
   STM_EVAL_LEDOff(LED4);

   //Initialize I/O expander
   IOE_Config();

   //Initialize LIS302DL accelerometer
   lis302dlInit();

   //Initialize LCD display
   STM324xG_LCD_Init();
   LCD_SetBackColor(Blue);
   LCD_SetTextColor(White);
   LCD_SetFont(&Font16x24);
   LCD_Clear(Blue);

   //Welcome message
   lcdSetCursor(0, 0);
   printf("Web Server Demo");

   //ADC3 initialization
   adc3Init();

   //Enable RNG peripheral clock
   RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
   //Enable RNG
   RNG_Cmd(ENABLE);

   //Generate a random seed
   for(i = 0; i < 32; i += 4)
   {
      //Wait for the RNG to contain a valid data
      while(RNG_GetFlagStatus(RNG_FLAG_DRDY) == RESET);

      //Get 32-bit random value
      value = RNG_GetRandomNumber();

      //Copy random value
      seed[i] = value & 0xFF;
      seed[i + 1] = (value >> 8) & 0xFF;
      seed[i + 2] = (value >> 16) & 0xFF;
      seed[i + 3] = (value >> 24) & 0xFF;
   }

   //PRNG initialization
   error = yarrowInit(&yarrowContext);
   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize PRNG!\r\n");
   }

   //Properly seed the PRNG
   error = yarrowSeed(&yarrowContext, seed, sizeof(seed));
   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to seed PRNG!\r\n");
   }

   //TCP/IP stack initialization
   error = tcpIpStackInit();
   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize TCP/IP stack!\r\n");
   }

   //Configure the first Ethernet interface
   interface = &netInterface[0];

   //Set interface name
   tcpIpStackSetInterfaceName(interface, "eth0");
   //Set host name
   tcpIpStackSetHostname(interface, "WebServerDemo");
   //Select the relevant network adapter
   tcpIpStackSetDriver(interface, &stm32f4x7EthDriver);
   tcpIpStackSetPhyDriver(interface, &dp83848PhyDriver);
   //Set external interrupt line driver
   tcpIpStackSetExtIntDriver(interface, &extIntDriver);
   //Set host MAC address
   macStringToAddr(APP_MAC_ADDR, &macAddr);
   tcpIpStackSetMacAddr(interface, &macAddr);

   //Initialize network interface
   error = tcpIpStackConfigInterface(interface);
   //Any error to report?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to configure interface %s!\r\n", interface->name);
   }

#if (IPV4_SUPPORT == ENABLED)
#if (APP_USE_DHCP == ENABLED)
   //Get default settings
   dhcpClientGetDefaultSettings(&dhcpClientSettings);
   //Set the network interface to be configured by DHCP
   dhcpClientSettings.interface = interface;
   //Disable rapid commit option
   dhcpClientSettings.rapidCommit = FALSE;

   //DHCP client initialization
   error = dhcpClientInit(&dhcpClientContext, &dhcpClientSettings);
   //Failed to initialize DHCP client?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize DHCP client!\r\n");
   }

   //Start DHCP client
   error = dhcpClientStart(&dhcpClientContext);
   //Failed to start DHCP client?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start DHCP client!\r\n");
   }
#else
   //Set IPv4 host address
   ipv4StringToAddr(APP_IPV4_HOST_ADDR, &ipv4Addr);
   ipv4SetHostAddr(interface, ipv4Addr);

   //Set subnet mask
   ipv4StringToAddr(APP_IPV4_SUBNET_MASK, &ipv4Addr);
   ipv4SetSubnetMask(interface, ipv4Addr);

   //Set default gateway
   ipv4StringToAddr(APP_IPV4_DEFAULT_GATEWAY, &ipv4Addr);
   ipv4SetDefaultGateway(interface, ipv4Addr);

   //Set primary and secondary DNS servers
   ipv4StringToAddr(APP_IPV4_PRIMARY_DNS, &ipv4Addr);
   ipv4SetDnsServer(interface, 0, ipv4Addr);
   ipv4StringToAddr(APP_IPV4_SECONDARY_DNS, &ipv4Addr);
   ipv4SetDnsServer(interface, 1, ipv4Addr);
#endif
#endif

#if (IPV6_SUPPORT == ENABLED)
#if (APP_USE_SLAAC == ENABLED)
   //Get default settings
   slaacGetDefaultSettings(&slaacSettings);
   //Set the network interface to be configured
   slaacSettings.interface = interface;

   //SLAAC initialization
   error = slaacInit(&slaacContext, &slaacSettings);
   //Failed to initialize SLAAC?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize SLAAC!\r\n");
   }

   //Start IPv6 address autoconfiguration process
   error = slaacStart(&slaacContext);
   //Failed to start SLAAC process?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start SLAAC!\r\n");
   }
#else
   //Set link-local address
   ipv6StringToAddr(APP_IPV6_LINK_LOCAL_ADDR, &ipv6Addr);
   ipv6SetLinkLocalAddr(interface, &ipv6Addr, IPV6_ADDR_STATE_VALID);

   //Set IPv6 prefix
   ipv6StringToAddr(APP_IPV6_PREFIX, &ipv6Addr);
   ipv6SetPrefix(interface, &ipv6Addr, APP_IPV6_PREFIX_LENGTH);

   //Set global address
   ipv6StringToAddr(APP_IPV6_GLOBAL_ADDR, &ipv6Addr);
   ipv6SetGlobalAddr(interface, &ipv6Addr, IPV6_ADDR_STATE_VALID);

   //Set router
   ipv6StringToAddr(APP_IPV6_ROUTER, &ipv6Addr);
   ipv6SetRouter(interface, &ipv6Addr);

   //Set primary and secondary DNS servers
   ipv6StringToAddr(APP_IPV6_PRIMARY_DNS, &ipv6Addr);
   ipv6SetDnsServer(interface, 0, &ipv6Addr);
   ipv6StringToAddr(APP_IPV6_SECONDARY_DNS, &ipv6Addr);
   ipv6SetDnsServer(interface, 1, &ipv6Addr);
#endif
#endif

   //Get default settings
   httpServerGetDefaultSettings(&httpServerSettings);
   //Bind HTTP server to the desired interface
   httpServerSettings.interface = &netInterface[0];
   //Listen to port 80
   httpServerSettings.port = HTTP_PORT;
   //Specify the server's root directory
   strcpy(httpServerSettings.rootDirectory, "/www/");
   //Set default home page
   strcpy(httpServerSettings.defaultDocument, "index.shtm");
   //Callback functions
   httpServerSettings.authCallback = httpServerAuthCallback;
   httpServerSettings.cgiCallback = httpServerCgiCallback;
   httpServerSettings.uriNotFoundCallback = httpServerUriNotFoundCallback;

   //HTTP server initialization
   error = httpServerInit(&httpServerContext, &httpServerSettings);
   //Failed to initialize HTTP server?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to initialize HTTP server!\r\n");
   }

   //Start HTTP server
   error = httpServerStart(&httpServerContext);
   //Failed to start HTTP server?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start HTTP server!\r\n");
   }

   //Start TCP echo service
   error = tcpEchoStart();
   //Failed to TCP echo service?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start TCP echo service!\r\n");
   }

   //Start UDP echo service
   error = udpEchoStart();
   //Failed to TCP echo service?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start UDP echo service!\r\n");
   }

   //Start TCP discard service
   error = tcpDiscardStart();
   //Failed to TCP echo service?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start TCP discard service!\r\n");
   }

   //Start UDP discard service
   error = udpDiscardStart();
   //Failed to TCP echo service?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start UDP discard service!\r\n");
   }

   //Start TCP chargen service
   error = tcpChargenStart();
   //Failed to TCP echo service?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start TCP chargen service!\r\n");
   }

   //Start UDP chargen service
   error = udpChargenStart();
   //Failed to TCP echo service?
   if(error)
   {
      //Debug message
      TRACE_ERROR("Failed to start UDP chargen service!\r\n");
   }

   //Create user task
   task = osCreateTask("User Task", userTask, NULL, 500, 1);
   //Failed to create the task?
   if(task == OS_INVALID_HANDLE)
   {
      //Debug message
      TRACE_ERROR("Failed to create task!\r\n");
   }

   //Create a task to blink the LED
   task = osCreateTask("Blink", blinkTask, NULL, 500, 1);
   //Failed to create the task?
   if(task == OS_INVALID_HANDLE)
   {
      //Debug message
      TRACE_ERROR("Failed to create task!\r\n");
   }

   //Start the execution of tasks
   osStartKernel();

   //This function should never return
   return 0;
}


/**
 * @brief HTTP authentication callback
 * @param[in] connection Handle referencing a client connection
 * @param[in] user NULL-terminated string specifying the user name
 * @param[in] uri NULL-terminated string containing the path to the requested resource
 * @return Access status (HTTP_ACCESS_ALLOWED, HTTP_ACCESS_DENIED,
 *   HTTP_BASIC_AUTH_REQUIRED or HTTP_DIGEST_AUTH_REQUIRED)
 **/

HttpAccessStatus httpServerAuthCallback(HttpConnection *connection,
   const char_t *user, const char_t *uri)
{
   HttpAccessStatus status;

   //Manage access rights
   if(pathMatch(uri, "/passwords.txt"))
   {
      //This file is not visible
      status = HTTP_ACCESS_DENIED;
   }
   else if(pathMatch(uri, "/config/*"))
   {
      //This directory is not visible
      status = HTTP_ACCESS_DENIED;
   }
   else if(pathMatch(uri, "/admin/*"))
   {
      //Only the administrator can access this directory
      if(!strcmp(user, "administrator"))
      {
         //Check the administrator password
         if(httpCheckPassword(connection, "password", HTTP_AUTH_MODE_BASIC))
            status = HTTP_ACCESS_ALLOWED;
         else
            status = HTTP_ACCESS_BASIC_AUTH_REQUIRED;
      }
      else
      {
         //Users other than administrator cannot access this directory
         status = HTTP_ACCESS_BASIC_AUTH_REQUIRED;
      }
   }
   else
   {
      //No restriction for other directories
      status = HTTP_ACCESS_ALLOWED;
   }

   //Return access status
   return status;
}


/**
 * @brief CGI callback function
 * @param[in] connection Handle referencing a client connection
 * @param[in] param NULL-terminated string that contains the CGI parameter
 * @return Error code
 **/

error_t httpServerCgiCallback(HttpConnection *connection,
   const char_t *param)
{
   static uint_t pageCounter = 0;
   uint_t length;

   //Underlying network interface
   NetInterface *interface = connection->socket->interface;

   //Check parameter name
   if(!strcasecmp(param, "PAGE_COUNTER"))
   {
      pageCounter++;
      sprintf(connection->buffer, "%u time%s", pageCounter, (pageCounter >= 2) ? "s" : "");
   }
   else if(!strcasecmp(param, "BOARD_NAME"))
   {
      strcpy(connection->buffer, "STM3240G-EVAL");
   }
   else if(!strcasecmp(param, "SYSTEM_TIME"))
   {
      systime_t time = osGetSystemTime();
      formatSystemTime(time, connection->buffer);
   }
   else if(!strcasecmp(param, "MAC_ADDR"))
   {
      macAddrToString(&interface->macAddr, connection->buffer);
   }
   else if(!strcasecmp(param, "IPV4_ADDR"))
   {
      ipv4AddrToString(interface->ipv4Config.addr, connection->buffer);
   }
   else if(!strcasecmp(param, "SUBNET_MASK"))
   {
      ipv4AddrToString(interface->ipv4Config.subnetMask, connection->buffer);
   }
   else if(!strcasecmp(param, "DEFAULT_GATEWAY"))
   {
      ipv4AddrToString(interface->ipv4Config.defaultGateway, connection->buffer);
   }
   else if(!strcasecmp(param, "IPV4_PRIMARY_DNS"))
   {
      ipv4AddrToString(interface->ipv4Config.dnsServer[0], connection->buffer);
   }
   else if(!strcasecmp(param, "IPV4_SECONDARY_DNS"))
   {
      ipv4AddrToString(interface->ipv4Config.dnsServer[1], connection->buffer);
   }
#if (IPV6_SUPPORT == ENABLED)
   else if(!strcasecmp(param, "LINK_LOCAL_ADDR"))
   {
      ipv6AddrToString(&interface->ipv6Config.linkLocalAddr, connection->buffer);
   }
   else if(!strcasecmp(param, "GLOBAL_ADDR"))
   {
      ipv6AddrToString(&interface->ipv6Config.globalAddr, connection->buffer);
   }
   else if(!strcasecmp(param, "IPV6_PREFIX"))
   {
      ipv6AddrToString(&interface->ipv6Config.prefix, connection->buffer);
      length = strlen(connection->buffer);
      sprintf(connection->buffer + length, "/%u", interface->ipv6Config.prefixLength);
   }
   else if(!strcasecmp(param, "ROUTER"))
   {
      ipv6AddrToString(&interface->ipv6Config.router, connection->buffer);
   }
   else if(!strcasecmp(param, "IPV6_PRIMARY_DNS"))
   {
      ipv6AddrToString(&interface->ipv6Config.dnsServer[0], connection->buffer);
   }
   else if(!strcasecmp(param, "IPV6_SECONDARY_DNS"))
   {
      ipv6AddrToString(&interface->ipv6Config.dnsServer[1], connection->buffer);
   }
#endif
   else
   {
      return __ERROR_INVALID_TAG;
   }

   //Get the length of the resulting string
   length = strlen(connection->buffer);

   //Send the contents of the specified environment variable
   return httpWriteStream(connection, connection->buffer, length);
}


/**
 * @brief URI not found callback
 * @param[in] connection Handle referencing a client connection
 * @param[in] uri NULL-terminated string containing the path to the requested resource
 * @return Error code
 **/

error_t httpServerUriNotFoundCallback(HttpConnection *connection,
   const char_t *uri)
{
   error_t error;
   uint_t n;
   char_t *buffer;

   //Process data.xml file?
   if(!strcasecmp(uri, "/data.xml"))
   {
      //Point to the scratch buffer
      buffer = connection->buffer + 384;

      //Format XML data
      n = sprintf(buffer, "<data>\r\n");
      n += sprintf(buffer + n, "  <ax>%d</ax>\r\n", ax);
      n += sprintf(buffer + n, "  <ay>%d</ay>\r\n", ay);
      n += sprintf(buffer + n, "  <az>%d</az>\r\n", az);
      n += sprintf(buffer + n, "  <adc>%u</adc>\r\n", adcValue);
      n += sprintf(buffer + n, "  <joystick>%u</joystick>\r\n", joystickState);

      //End of XML data
      n += sprintf(buffer + n, "</data>\r\n");

      //Format HTTP response header
      connection->response.version = connection->request.version;
      connection->response.statusCode = 200;
      connection->response.keepAlive = connection->request.keepAlive;
      connection->response.noCache = TRUE;
      connection->response.contentType = mimeGetType(".xml");
      connection->response.chunkedEncoding = FALSE;
      connection->response.contentLength = n;

      //Send the header to the client
      error = httpWriteHeader(connection);
      //Any error to report?
      if(error) return error;

      //Send response body
      error = httpWriteStream(connection, buffer, n);
      //Any error to report?
      if(error) return error;

      //Properly close output stream
      error = httpCloseStream(connection);
      //Return status code
      return error;
   }
   //Process send_mail.xml file?
   else if(!strcasecmp(uri, "/send_mail.xml"))
   {
      char *separator;
      char *property;
      char *value;
      char *p;
      SmtpAuthInfo authInfo;
      SmtpMail mail;
      SmtpMailAddr recipients[4];

      //Initialize structures to zero
      memset(&authInfo, 0, sizeof(authInfo));
      memset(&mail, 0, sizeof(mail));
      memset(recipients, 0, sizeof(recipients));

      //Set the relevant PRNG algorithm to be used
      authInfo.prngAlgo = YARROW_PRNG_ALGO;
      authInfo.prngContext = &yarrowContext;

      //Set email recipients
      mail.recipients = recipients;
      //Point to the scratch buffer
      buffer = connection->buffer;

      //Start of exception handling block
      do
      {
         //Process HTTP request body
         while(1)
         {
            //Read the HTTP request body until an ampersand is encountered
            error = httpReadStream(connection, buffer,
               HTTP_SERVER_BUFFER_SIZE - 1, &n, HTTP_FLAG_BREAK('&'));
            //End of stream detected?
            if(error) break;

            //Properly terminate the string with a NULL character
            buffer[n] = '\0';

            //Remove the trailing ampersand
            if(n > 0 && buffer[n - 1] == '&')
               buffer[--n] = '\0';

            //Decode the percent-encoded string
            httpDecodePercentEncodedString(buffer, buffer, HTTP_SERVER_BUFFER_SIZE);
            //Check whether a separator is present
            separator = strchr(buffer, '=');

            //Separator found?
            if(separator)
            {
               //Split the line
               *separator = '\0';
               //Get property name and value
               property = strTrimWhitespace(buffer);
               value = strTrimWhitespace(separator + 1);

               //Check property name
               if(!strcasecmp(property, "server"))
               {
                  //Save server name
                  authInfo.serverName = strDuplicate(value);
               }
               else if(!strcasecmp(property, "port"))
               {
                  //Save the server port to be used
                  authInfo.serverPort = atoi(value);
               }
               else if(!strcasecmp(property, "userName"))
               {
                  //Save user name
                  authInfo.userName = strDuplicate(value);
               }
               else if(!strcasecmp(property, "password"))
               {
                  //Save password
                  authInfo.password = strDuplicate(value);
               }
               else if(!strcasecmp(property, "useTls"))
               {
                  //Open a secure SSL/TLS session?
                  authInfo.useTls = TRUE;
               }
               else if(!strcasecmp(property, "recipient"))
               {
                  //Split the recipient address list
                  value = strtok_r(value, ", ", &p);

                  //Loop through the list
                  while(value != NULL)
                  {
                     //Save recipient address
                     recipients[mail.recipientCount].name = NULL;
                     recipients[mail.recipientCount].addr = strDuplicate(value);
                     recipients[mail.recipientCount].type = SMTP_RCPT_TYPE_TO;
                     //Get the next item in the list
                     value = strtok_r(NULL, ", ", &p);

                     //Increment the number of recipients
                     if(++mail.recipientCount >= arraysize(recipients))
                        break;
                  }
               }
               else if(!strcasecmp(property, "from"))
               {
                  //Save sender address
                  mail.from.name = NULL;
                  mail.from.addr = strDuplicate(value);
               }
               else if(!strcasecmp(property, "date"))
               {
                  //Save current time
                  mail.dateTime = strDuplicate(value);
               }
               else if(!strcasecmp(property, "subject"))
               {
                  //Save mail subject
                  mail.subject = strDuplicate(value);
               }
               else if(!strcasecmp(property, "body"))
               {
                  //Save mail body
                  mail.body = strDuplicate(value);
               }
            }
         }

         //Propagate exception if necessary
         if(error != __ERROR_END_OF_STREAM)
            break;

         //Send mail
         error = smtpSendMail(&authInfo, &mail);

         //Point to the scratch buffer
         buffer = connection->buffer + 384;
         //Format XML data
         n = sprintf(buffer, "<data>\r\n  <status>");

         if(error == __NO_ERROR)
            n += sprintf(buffer + n, "Mail successfully sent!\r\n");
         else if(error == __ERROR_NAME_RESOLUTION_FAILED)
            n += sprintf(buffer + n, "Cannot resolve SMTP server name!\r\n");
         else if(error == __ERROR_AUTHENTICATION_FAILED)
            n += sprintf(buffer + n, "Authentication failed!\r\n");
         else if(error == __ERROR_UNEXPECTED_RESPONSE)
            n += sprintf(buffer + n, "Unexpected response from SMTP server!\r\n");
         else
            n += sprintf(buffer + n, "Failed to send mail (error %d)!\r\n", error);

         n += sprintf(buffer + n, "</status>\r\n</data>\r\n");

         //Format HTTP response header
         connection->response.version = connection->request.version;
         connection->response.statusCode = 200;
         connection->response.keepAlive = connection->request.keepAlive;
         connection->response.noCache = TRUE;
         connection->response.contentType = mimeGetType(".xml");
         connection->response.chunkedEncoding = FALSE;
         connection->response.contentLength = n;

         //Send the header to the client
         error = httpWriteHeader(connection);
         //Any error to report?
         if(error) break;

         //Send response body
         error = httpWriteStream(connection, buffer, n);
         //Any error to report?
         if(error) break;

         //Properly close output stream
         error = httpCloseStream(connection);
         //Any error to report?
         if(error) break;

         //End of exception handling block
      } while(0);

      //Free previously allocated memory
      osFreeMem((void *) authInfo.serverName);
      osFreeMem((void *) authInfo.userName);
      osFreeMem((void *) authInfo.password);
      osFreeMem((void *) recipients[0].addr);
      osFreeMem((void *) mail.from.addr);
      osFreeMem((void *) mail.dateTime);
      osFreeMem((void *) mail.subject);
      osFreeMem((void *) mail.body);

      //Return status code
      return error;
   }
   else
   {
      return __ERROR_NOT_FOUND;
   }
}
