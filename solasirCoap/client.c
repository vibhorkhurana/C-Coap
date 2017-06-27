#include "coap.h"
#include <stdio.h>

/*
 * The response handler
 */ 
static void
message_handler(struct coap_context_t *ctx, const coap_endpoint_t *local_interface, 
                const coap_address_t *remote, coap_pdu_t *sent, coap_pdu_t *received, 
                const coap_tid_t id) 
{
	unsigned char* data;
	size_t         data_len;
	if (COAP_RESPONSE_CLASS(received->hdr->code) == 2) 
	{
		if (coap_get_data(received, &data_len, &data))
		{
			coap_show_pdu(received);
			printf("Received: %s\n", data);
		}
	}
}

int main(int argc, char* argv[])
{
	if(argc<3)
	{
	fprintf(stderr,"Usage:%s <method> <resource> [payload]\n",argv[0]);
	exit(1);
	}
	coap_context_t*   ctx;
	coap_address_t    dst_addr, src_addr;
	static coap_uri_t uri;
	fd_set            readfds; 
	coap_pdu_t*       request;
	char       server_uri[64] = "coap://127.0.0.1/";
	unsigned char     req_method = 1;
	if(strcasecmp(argv[1],"get")==0)
		req_method=1;
	else if(strcasecmp(argv[1],"post")==0)
		req_method=2;
	else if(strcasecmp(argv[1],"put")==0)
		req_method=3;
	else if(strcasecmp(argv[1],"delete")==0)
		req_method=4;
	else {
		fprintf(stderr,"unknown request method:%s\n",argv[1]);
		exit(2);
	}
	if(argc==3)
		strcat(server_uri,argv[2]);
	else
		strcat(server_uri,"hello");
	/* Prepare coap socket*/
	coap_set_log_level(LOG_INFO);
	coap_address_init(&src_addr);
	src_addr.addr.sin.sin_family      = AF_INET;
	src_addr.addr.sin.sin_port        = htons(0);
	src_addr.addr.sin.sin_addr.s_addr = inet_addr("0.0.0.0");
	ctx = coap_new_context(&src_addr);
	/* The destination endpoint */
	coap_address_init(&dst_addr);
	dst_addr.addr.sin.sin_family      = AF_INET;
	dst_addr.addr.sin.sin_port        = htons(5683);
	dst_addr.addr.sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	/* Prepare the request */
	coap_split_uri(server_uri, strlen(server_uri), &uri);
	request            = coap_new_pdu();	
	request->hdr->type = COAP_MESSAGE_CON;
	request->hdr->id   = coap_new_message_id(ctx);
	request->hdr->code = req_method;
	coap_add_option(request, COAP_OPTION_URI_PATH, uri.path.length, uri.path.s);
	if(req_method==2 || req_method==3)
	{
		if(argc>=4)
			coap_add_data(request,strlen(argv[3]),argv[3]);
		else
		{
			char buf[128];
			printf("enter payload for PUT/POST method\n");
			scanf("%s",buf);
			coap_add_data(request,strlen(buf),buf);
		}
	}
	/* Set the handler and send the request */
	coap_register_response_handler(ctx, message_handler);
	coap_send_confirmed(ctx, ctx->endpoint, &dst_addr, request);
	FD_ZERO(&readfds);
	FD_SET( ctx->sockfd, &readfds );
	int result = select( FD_SETSIZE, &readfds, 0, 0, NULL );
	if ( result < 0 ) /* socket error */
	{
		exit(EXIT_FAILURE);
	} 
	else if ( result > 0 && FD_ISSET( ctx->sockfd, &readfds )) /* socket read*/
	{	 
			coap_read( ctx );       
	} 
  return 0;
}
