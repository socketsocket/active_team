
#include <stdio.h>                    /* C-stdio library.             */
#include <string.h>                   /* string functions.            */
#include <stdlib.h>                   /* stdlib functions.            */
#include <errno.h>                    /* errno values.                */
 
 
#define LINELEN  80                   /* Max length of line.          */
 
 
 
/**********************************************************************/
/*                                                                    */
/* Function Name: writeData()                                         */
/*                                                                    */
/* Descriptive Name: Function is used to print the data to the        */
/* browser.  The data is printed 80 characters/line to provide        */
/* a neat and readable output.                                        */
/*                                                                    */
/* HTTP Server Environment variables:                                 */
/* ----------------------------------                                 */
/*                                                                    */
/* Standard Input:                                                    */
/* ---------------                                                    */
/*                                                                    */
/* Standard Output:                                                   */
/* ----------------                                                   */
/* All data directed to Standard output is sent using printf() or     */
/* putchar().  Standard output is written with html text.             */
/*                                                                    */
/*                                                                    */
/* Input: ptrToData : A pointer to the data to write to stdout.       */
/*        dataLen   : Length of data buffer.                          */
/*                                                                    */
/* Output: Data buffer written to stdout.                             */
/*                                                                    */
/* Exit Normal:                                                       */
/*                                                                    */
/* Exit Error: None                                                   */
/*                                                                    */
/**********************************************************************/
void writeData(char* ptrToData, int dataLen)
{
 
  div_t  insertBreak;
  int    i;
 
  /*------------------------------------------------------------------*/
  /* Write dataLen bytes of data from ptrToData.                      */
  /*------------------------------------------------------------------*/
  for (i=1; i<= dataLen; i++) {
 
      putchar(*ptrToData);
      ptrToData++;
 
      /*--------------------------------------------------------------*/
      /* Print a break after every 80 characters.                     */
      /*--------------------------------------------------------------*/
      insertBreak = div(i, LINELEN);
      if ( insertBreak.rem == 0 )       
          printf("<br>");
 
  }
 
  return;
 
}
 
 
 
 
 
/**********************************************************************/
/*                                                                    */
/* Function Name: main()                                              */
/*                                                                    */
/* Descriptive Name: A sample of the method used for iSeries ILE/C to */
/* read standard input, write standard output and check environment   */
/* variables; SERVER_SOFTWARE, REQUEST_METHOD, CONTENT_LENGTH, etc.   */
/*                                                                    */
/* HTTP Server Environment variables:                                 */
/* ----------------------------------                                 */
/* The C function call, getenv, is used to read iSeries server        */
/* environment variables.  The value of the argument is a (char *)    */
/* pointer with the name of the environment variable. The value of    */
/* the environment variable is always returned as a string pointer.   */
/* The value may need to be converted to be used; that is             */
/* CONTENT_LENGTH needs to be converted to int using atoi().          */
/*                                                                    */
/* Standard Input:                                                    */
/* ---------------                                                    */
/* CONTENT_LENGTH is used to determine the amount of data to be       */
/* read from standard input with fread().  The standard input is      */
/* considered to be a stream of bytes up to CONTENT_LENGTH bytes. The */
/* standard input can be read with any file input stream function up  */
/* to and including CONTENT_LENGTH bytes.  Reading more than          */
/* CONTENT_LENGTH bytes is not defined.                               */
/*                                                                    */
/* Standard Output:                                                   */
/* ----------------                                                   */
/* All data directed to Standard output is using writeData().         */
/*                                                                    */
/* Standard output is written with html text which includes HTTP      */
/* header lines identifying the content type of the data written and  */
/* HTTP response headers.  This MUST be followed by a blank line(\n\n)*/
/* before writing any html text.  This indicates the end of the       */
/* header and the start of text that is served from the server.       */
/* This text is usually html but can be plain/text.                   */
/*                                                                    */
/* Input: Data read from standard input or QUERY_STRING that is       */
/*        entered in an HTML form.                                    */
/*                                                                    */
/* Output: The data read from standard input is written as is to      */
/*         standard output.  This information would then be served by */
/*         the HTTP server.                                           */
/*                                                                    */
/* Exit Normal:                                                       */
/*                                                                    */
/* Exit Error: None                                                   */
/*                                                                    */
/**********************************************************************/
void main()
{
 
  char   *stdInData;                   /* Input buffer.               */
  char   *queryString;                 /* Query String env variable   */
  char   *requestMethod;               /* Request method env variable */
  char   *serverSoftware;              /* Server Software env variable*/
  char   *contentLenString;            /* Character content length.   */
  int    contentLength;                /* int content length          */
  int    bytesRead;                    /* number of bytes read.       */
  int    queryStringLen;               /* Length of QUERY_STRING      */
 
  /*------------------------------------------------------------------*/
  /* The "Content-type" is the minimum request header that must be    */
  /* written to standard output.  It describes the type of data that  */
  /* follows.                                                         */
  /*------------------------------------------------------------------*/
  printf("Content-type: text/html\n");
 
  /*------------------------------------------------------------------*/
  /* VERY IMPORTANT! An extra newline must be written                 */
  /* after the request header.  In this case the request header is    */
  /* only the Content-type.  This tells the HTTP server that the      */
  /* request header is ended and the data follows.                    */
  /*------------------------------------------------------------------*/
  printf("\n");
 
  /*------------------------------------------------------------------*/
  /* This html text consists of a head and body section.  The head    */
  /* section has a title for the document.  The body section will     */
  /* contain standard input, QUERY_STRING, CONTENT_LENGTH,            */
  /* SERVER_SOFTWARE and REQUEST_METHOD.                              */
  /*------------------------------------------------------------------*/
  printf("<html>\n");
  printf("<head>\n");
  printf("<title>\n");
  printf("Sample iSeries HTTP Server CGI program\n");
  printf("</title>\n");
  printf("</head>\n");
  printf("<body>\n");
  printf("<h1>Sample iSeries ILE/C program.</h1>\n");
  printf("<br>This is sample output writing in iSeries ILE/C\n");
  printf("<br>as a sample of CGI programming.  This program reads\n");
  printf("<br>the input data from Query_String environment\n");
  printf("<br>variable when the Request_Method is GET and reads\n");
  printf("<br>standard input when the Request_Method is POST.\n");
 
  /*------------------------------------------------------------------*/
  /* Get and write the REQUEST_METHOD to stdout.                      */
  /*------------------------------------------------------------------*/
  requestMethod = getenv("REQUEST_METHOD");
  if ( requestMethod )
      printf("<h4>REQUEST_METHOD:</h4>%s\n", requestMethod);
  else
      printf("Error extracting environment variable REQUEST_METHOD.\n");
 
  /*------------------------------------------------------------------*/
  /* html form data can be provided to the CGI program either on      */
  /* stdin or in environment variable QUERY_STRING.  This can be      */
  /* determined by examining REQUEST_METHOD.                          */
  /*------------------------------------------------------------------*/
  if ( strcmp(requestMethod,"POST") == 0 ) {
 
      /*--------------------------------------------------------------*/
      /* The REQUEST_METHOD is "POST".  The environment variable      */
      /* CONTENT_LENGTH will tell us how many bytes of data to read   */
      /* from stdin.  Note: CONTENT_LENGTH must be convert to an int. */
      /*--------------------------------------------------------------*/
      contentLenString = getenv("CONTENT_LENGTH");
      contentLength = atoi(contentLenString);
 
      /*--------------------------------------------------------------*/
      /* Write CONTENT_LENGTH to stdout.                              */
      /*--------------------------------------------------------------*/
      printf("<h4>CONTENT_LENGTH:</h4>%i<br><br>\n",contentLength);
 
      if ( contentLength ) {
 
          /*----------------------------------------------------------*/
          /* Allocate and set memory to read stdin data into.         */
          /*----------------------------------------------------------*/
          stdInData = malloc(contentLength);
          if ( stdInData )
              memset(stdInData, 0x00, contentLength);
          else
              printf("ERROR: Unable to allocate memory\n");
          /*----------------------------------------------------------*/
          /* A CGI program MUST read standard input as a stream       */
          /* file only up to and including CONTENT_LENGTH bytes.      */
          /* Never should a program read more than CONTENT_LENGTH     */
          /* bytes.  A CGI program that reads standard input must     */
          /* never depend on an end of file flag.  This will cause    */
          /* unpredictable results when the CGI program reads         */
          /* standard input.                                          */
          /*----------------------------------------------------------*/
          printf("<h4>Server standard input:</h4>\n");
          bytesRead = fread((char*)stdInData, 1, contentLength, stdin);
 
          /*----------------------------------------------------------*/
          /* If we successfully read all bytes from stdin, format and */
          /* write the data to stdout using the writeData function.   */
          /*----------------------------------------------------------*/
          if ( bytesRead == contentLength )
              writeData(stdInData, bytesRead);
          else
              printf("<br>Error reading standard input\n");
 
          /*----------------------------------------------------------*/
          /* Free the storage allocated to hold the stdin data.       */
          /*----------------------------------------------------------*/
          free(stdInData);
 
      } else
          printf("<br><br><b>There is no standard input data.</b>");
 
  } else if (strcmp(requestMethod, "GET") == 0 ) {
      /*--------------------------------------------------------------*/
      /* The REQUEST_METHOD is "GET".  The environment variable       */
      /* QUERY_STRING will contain the form data.                     */
      /*--------------------------------------------------------------*/
      queryString = getenv("QUERY_STRING");
      if ( queryString ) {
 
          /*----------------------------------------------------------*/
          /* Write the QUERY_STRING data to stdout.                   */
          /*----------------------------------------------------------*/
          printf("<h4>Server input read from QUERY_STRING:</h4>");
          queryStringLen = strlen(queryString);
          if ( queryStringLen )
              writeData(queryString, queryStringLen);
          else
              printf("<b>There is no data in QUERY_STRING.</b>");
 
      } else
          printf("<br>Error getting QUERY_STRING variable.");
 
  } else
      printf("<br><h2>ERROR: Invalid REQUEST_METHOD.</h2>");
 
  /*------------------------------------------------------------------*/
  /* Write break and paragraph html tag to stdout.                    */
  /*------------------------------------------------------------------*/
printf("<br><p>\n");
 
 /*------------------------------------------------------------------*/
  /* Write the SERVER_SOFTWARE environment variable to stdout.        */
  /*------------------------------------------------------------------*/
  serverSoftware = getenv("SERVER_SOFTWARE");
  if ( serverSoftware )
      printf("<h4>SERVER_SOFTWARE:</h4>%s\n", serverSoftware);
  else
      printf("<h4>Server Software is NULL</h4>");
 
  /*-----------------------------------------------------------------*/
  /* Write the closing tags on HTML document.                        */
  /*-----------------------------------------------------------------*/
  printf("</p>\n");
  printf("</body>\n");
  printf("</html>\n");
 
  return;
}