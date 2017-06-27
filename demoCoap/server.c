#include <stdio.h>
#include <unistd.h>
#include <coap.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
static void hello_handler(coap_context_t *ctx,
                        struct coap_resource_t *resource,
                        const coap_endpoint_t *local_interface,
                        coap_address_t *peer,
                        coap_pdu_t * request,
                        str *token,
                        coap_pdu_t *response){
                        
    unsigned char buf[3];
    const char* response_data = "Hello World!";
    response->hdr->code           = COAP_RESPONSE_CODE(205);
	coap_add_option(response, COAP_OPTION_CONTENT_TYPE, coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);
	coap_add_data  (response, strlen(response_data), (unsigned char *)response_data);

}

int main()
{
    coap_context_t *ctx;
    coap_address_t dst_addr, src_addr;
    int ret;
    fd_set activefds,readfds;

    coap_set_log_level(LOG_INFO);
    coap_address_init(&src_addr);
    src_addr.addr.sin.sin_family = AF_INET;
    src_addr.addr.sin.sin_port = htons(5683);
    src_addr.addr.sin.sin_addr.s_addr = inet_addr("0.0.0.0");
    ctx = coap_new_context(&src_addr);

    coap_resource_t *hello_resource;
    hello_resource=coap_resource_init((unsigned char *)"hello",5,0);
    coap_register_handler(hello_resource, COAP_REQUEST_GET,hello_handler);
    coap_add_resource(ctx,hello_resource);
    
    FD_ZERO(&activefds);
    FD_SET(ctx->sockfd,&activefds);
    while(1)
    {
        readfds=activefds;
        ret=select(FD_SETSIZE, &readfds, NULL, NULL, NULL);
        if(ret<0){
            perror("select");
        }
        if(FD_ISSET(ctx->sockfd,&readfds)){
        coap_read(ctx);
    }
}
    
    return 0;
}