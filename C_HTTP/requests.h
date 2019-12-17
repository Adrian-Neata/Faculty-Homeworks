#ifndef _REQUESTS_
#define _REQUESTS_

char *compute_get_request(char *host, char *url, char *url_params, char *authorization, char *cookies);
char *compute_post_request(char *host, char *url, char *authorization, char *cookies, char *contentType, char *form_data);

#endif
