/*Para compilar usar:
gcc -o bin/server server.c -lulfius -ljansson
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ulfius.h>
#include <locale.h>
#include <orcania.h>
#include <jansson.h>
#include "include/cercaVirtual.h"

#define PORT 8080
int id = 0;
char *json;


int callback_default (const struct _u_request * request, struct _u_response * response, void * user_data);
int callback_get_with_params(const struct _u_request *request, struct _u_response *response, void *user_data);
int createToken(const struct _u_request *request, struct _u_response *response, void *user_data);
int callback_post(const struct _u_request *request, struct _u_response *response, void *user_data);
int callback_define_cercaVirtual(const struct _u_request *request, struct _u_response *response, void *user_data);


int main(void) {
    setlocale(LC_ALL, "");
    char txt[] = "{\"Ip\": \"8.8.8.8\",\"Cow\": [{\"ID\": 1,	\"Localizacao\": [{\"Horario\": \"2023-06-06 23:39:40.845\",\"Latitude\": -29.6928309,\"Longitude\": -53.8094293},{\"Horario\": \"2023-06-06 23:39:40.845\",\"Latitude\": 45.516,\"Longitude\": -122.636}]},{\"ID\": 2,\"Localizacao\": [{\"Horario\": \"2023-06-06 23:39:40.845\",\"Latitude\": 45.516,\"Longitude\": -122.636},{\"Horario\": \"2023-06-06 23:39:40.845\",\"Latitude\": 45.516,\"Longitude\": -122.636}]}]}";
    json = txt;


    struct _u_instance instance;

    // Initialize instance with the port number
    if (ulfius_init_instance(&instance, PORT, NULL, NULL) != U_OK) {
        fprintf(stderr, "Error ulfius_init_instance, abort\n");
        return(1);
    }

    // Endpoint list declaration
    ulfius_add_endpoint_by_val(&instance, "POST", "/cercaVirtual", NULL, 0, &callback_define_cercaVirtual, NULL);
    ulfius_add_endpoint_by_val(&instance, "POST", "/post", NULL, 0, &callback_post, NULL);
    ulfius_add_endpoint_by_val(&instance, "GET", "/createToken", NULL, 0, &createToken, NULL);
    ulfius_add_endpoint_by_val(&instance, "GET", "/localization",NULL, 0, &callback_get_with_params, NULL);
    ulfius_set_default_endpoint(&instance, &callback_default, NULL);// default_endpoint declaration

    // Start the framework
    if (ulfius_start_framework(&instance) == U_OK) {
        printf("Start framework on port %d\n", instance.port);

        // Wait for the user to press <enter> on the console to quit the application
        getchar();
    } else {
        fprintf(stderr, "Error starting framework\n");
    }
    printf("End framework\n");

    ulfius_stop_framework(&instance);
    ulfius_clean_instance(&instance);

    return 0;
}

int callback_get_with_params(const struct _u_request *request, struct _u_response *response, void *user_data) {
    const char *param1 = u_map_get(request->map_url, "startDate");
    const char *param2 = u_map_get(request->map_url, "endDate");

    if (param1 != NULL && param2 != NULL) {
        // Lógica para lidar com os parâmetros recebidos
        printf("startDate: %s\n", param1);  //2023-06-11 11:45:12.456
        printf("endDate: %s\n", param2);  //

        // Exemplo: retornar uma resposta simples
        ulfius_set_string_body_response(response, 200, json);
    } else {
        printf("...");
        // Parâmetros ausentes ou inválidos
        ulfius_set_string_body_response(response, 400, "Parâmetros inválidos");
    }

    return U_CALLBACK_CONTINUE;
}


int createToken(const struct _u_request *request, struct _u_response *response, void *user_data) {
    const char *param1 = u_map_get(request->map_url, "Ip");
    char token[64];
    /* Gerar o token */
    id++;
    sprintf(token,"%d",id);
    /* Gerar o token */
    ulfius_set_string_body_response(response, 200, token);


    return U_CALLBACK_CONTINUE;
}



/**
 * Default callback function called if no endpoint has a match
 */
int callback_default (const struct _u_request * request, struct _u_response * response, void * user_data) {
    (void)(request);
    (void)(user_data);
    ulfius_set_string_body_response(response, 404, "Page not found, do what you want");
    return U_CALLBACK_CONTINUE;
}

int callback_post(const struct _u_request *request, struct _u_response *response, void *user_data) {
    json_t *json_body = ulfius_get_json_body_request(request,NULL);
    if (json_body != NULL) {
        char *json_str = json_dumps(json_body, JSON_INDENT(2));
        printf("JSON recebido:\n%s\n", json_str);
        free(json_str);
    } else {
        printf("Falha ao obter o JSON do corpo da solicitação.\n");
    }

    return U_CALLBACK_CONTINUE;
}

int callback_define_cercaVirtual(const struct _u_request *request, struct _u_response *response, void *user_data) {
    json_t *json_body = ulfius_get_json_body_request(request,NULL);
    if (json_body != NULL) {
        char *json_str = json_dumps(json_body, JSON_INDENT(2));
        printf("JSON recebido:\n%s\n", json_str);

        struct Point pontoRecebido = {-27.862716104479084, -54.467711144344776};

        if(isInsidePolygon(pontoRecebido, polygon, 4)){
            printf("O ponto está dentro do polígono.\n");
            ulfius_set_string_body_response(response, 200,"ok");

        }else{
            printf("O ponto está fora do polígono.\n");

        }

        free(json_str);
    } else {
        printf("Falha ao obter o JSON do corpo da solicitação.\n");
    }

    return U_CALLBACK_CONTINUE;
}


/* Exemplo de post com body x-www-form-urlencoded */
/*
int callback_post_test (const struct _u_request * request, struct _u_response * response, void * user_data) {
  char * post_params = print_map(request->map_post_body);
  printf("\nRecebido: %s\n", post_params);
  char * response_body = msprintf("Hello World!\n%s", post_params);
  (void)(user_data);
  ulfius_set_string_body_response(response, 200, response_body);
  o_free(response_body);
  o_free(post_params);
  return U_CALLBACK_CONTINUE;
}

char * print_map(const struct _u_map * map) {
  char * line, * to_return = NULL;
  const char **keys, * value;
  int len, i;
  if (map != NULL) {
    keys = u_map_enum_keys(map);
    for (i=0; keys[i] != NULL; i++) {
      value = u_map_get(map, keys[i]);
      len = snprintf(NULL, 0, "key is %s, value is %s", keys[i], value);
      line = o_malloc((size_t)(len+1));
      snprintf(line, (size_t)(len+1), "key is %s, value is %s", keys[i], value);
      if (to_return != NULL) {
        len = (int)(o_strlen(to_return) + o_strlen(line) + 1);
        to_return = o_realloc(to_return, (size_t)(len+1));
        if (o_strlen(to_return) > 0) {
          strcat(to_return, "\n");
        }
      } else {
        to_return = o_malloc((o_strlen(line) + 1));
        to_return[0] = 0;
      }
      strcat(to_return, line);
      o_free(line);
    }
    return to_return;
  } else {
    return NULL;
  }
}
*/