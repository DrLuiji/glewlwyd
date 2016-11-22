/**
 *
 * Glewlwyd OAuth2 Authorization Server
 *
 * OAuth2 authentiation server
 * Users are authenticated with a LDAP server
 * or users stored in the database 
 * 
 * Declarations for constants and prototypes
 *
 * Copyright 2016 Nicolas Mora <mail@babelouest.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU GENERAL PUBLIC LICENSE
 * License as published by the Free Software Foundation;
 * version 3 of the License.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU GENERAL PUBLIC LICENSE for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __GLEWLWYD_H_
#define __GLEWLWYD_H_

#include <jansson.h>
#include <jwt.h>

/** Angharad libraries **/
#include <ulfius.h>
#include <yder.h>

#define _HOEL_MARIADB
#define _HOEL_SQLITE
#include <hoel.h>

#define _GLEWLWYD_VERSION 0.2
#define GLEWLWYD_LOG_NAME "Glewlwyd"

// Configuration management
#define GLEWLWYD_DEFAULT_PREFIX             "glewlwyd"
#define GLEWLWYD_DEFAULT_PORT               4593
#define GLEWLWYD_REFRESH_TOKEN_EXP_DEFAULT  1209600
#define GLEWLWYD_ACCESS_TOKEN_EXP_DEFAULT   3600
#define GLEWLWYD_SESSION_KEY_DEFAULT        "GLEWLWYD_SESSION_ID"
#define GLEWLWYD_SESSION_EXPIRATION_DEFAULT 604800

#define GLEWLWYD_RUNNING  0
#define GLEWLWYD_STOP     1
#define GLEWLWYD_ERROR    2

#define G_OK                 0
#define G_ERROR              1
#define G_ERROR_UNAUTHORIZED 2
#define G_ERROR_PARAM        3
#define G_ERROR_DB           4
#define G_ERROR_MEMORY       5

#define GLEWLWYD_TABLE_CLIENT_USER_SCOPE         "g_client_user_scope"
#define GLEWLWYD_TABLE_CLIENT_AUTHORIZATION_TYPE "g_client_authorization_type"
#define GLEWLWYD_TABLE_RESOURCE_SCOPE            "g_resource_scope"
#define GLEWLWYD_TABLE_USER_SCOPE                "g_user_scope"
#define GLEWLWYD_TABLE_REFRESH_TOKEN             "g_refresh_token"
#define GLEWLWYD_TABLE_ACCESS_TOKEN              "g_access_token"
#define GLEWLWYD_TABLE_RESOURCE                  "g_resource"
#define GLEWLWYD_TABLE_REDIRECT_URI              "g_redirect_uri"
#define GLEWLWYD_TABLE_CLIENT                    "g_client"
#define GLEWLWYD_TABLE_AUTHORIZATION_TYPE        "g_authorization_type"
#define GLEWLWYD_TABLE_SCOPE                     "g_scope"
#define GLEWLWYD_TABLE_USER                      "g_user"

#define GLEWLWYD_AUHORIZATION_TYPE_AUTHORIZATION_CODE                  0
#define GLEWLWYD_AUHORIZATION_TYPE_CODE                                1
#define GLEWLWYD_AUHORIZATION_TYPE_IMPLICIT                            2
#define GLEWLWYD_AUHORIZATION_TYPE_RESOURCE_OWNER_PASSWORD_CREDENTIALS 3
#define GLEWLWYD_AUHORIZATION_TYPE_CLIENT_CREDENTIALS                  4

int global_handler_variable;

struct _auth_ldap {
  char * uri;
  char * bind_dn;
  char * bind_passwd;
  char * filter;
  char * login_property;
  char * scope_property;
  char * base_search;
};

struct config_elements {
  char *                 config_file;
  char *                 url_prefix;
  unsigned long          log_mode;
  unsigned long          log_level;
  char *                 log_file;
  char *                 allow_origin;
  char *                 static_files_path;
  char *                 static_files_prefix;
  unsigned int           use_scope;
  unsigned int           has_auth_database;
  unsigned int           has_auth_ldap;
  struct _auth_ldap *    auth_ldap;
  struct _u_map *        mime_types;
  struct _h_connection * conn;
  struct _u_instance   * instance;
  jwt_t *                jwt;
  char *                 jwt_decode_key;
  char *                 session_key;
  unsigned int           session_expiration;
  unsigned int           refresh_token_expiration;
  unsigned int           access_token_expiration;
};

/**
 * decode a u_map into a string
 */
char * print_map(const struct _u_map * map);

int  build_config_from_args(int argc, char ** argv, struct config_elements * config);
int  build_config_from_file(struct config_elements * config);
int  check_config(struct config_elements * config);
void exit_handler(int handler);
void exit_server(struct config_elements ** config, int exit_value);
void print_help(FILE * output);
const char * get_filename_ext(const char *path);
char * get_file_content(const char * file_path);
char * str2md5(const char * str, int length);
char *url_decode(char *str);
char *url_encode(char *str);
char * generate_query_parameters(const struct _u_request * request);
const char * get_ip_source(const struct _u_request * request);

int check_auth_type_auth_code_grant (const struct _u_request * request, struct _u_response * response, void * user_data);
int check_auth_type_access_token_request (const struct _u_request * request, struct _u_response * response, void * user_data);
int check_auth_type_implicit_grant (const struct _u_request * request, struct _u_response * response, void * user_data);
int check_auth_type_resource_owner_pwd_cred (const struct _u_request * request, struct _u_response * response, void * user_data);
int check_auth_type_client_credentials_grant (const struct _u_request * request, struct _u_response * response, void * user_data);

int callback_glewlwyd_check_auth_session (const struct _u_request * request, struct _u_response * response, void * user_data);
int callback_glewlwyd_authorization (const struct _u_request * request, struct _u_response * response, void * user_data);
int callback_glewlwyd_token (const struct _u_request * request, struct _u_response * response, void * user_data);
int callback_glewlwyd_user_authorization (const struct _u_request * request, struct _u_response * response, void * user_data);
int callback_glewlwyd_user_scope_grant (const struct _u_request * request, struct _u_response * response, void * user_data);
int callback_glewlwyd_options (const struct _u_request * request, struct _u_response * response, void * user_data);
int callback_glewlwyd_static_file (const struct _u_request * request, struct _u_response * response, void * user_data);
int callback_glewlwyd_root (const struct _u_request * request, struct _u_response * response, void * user_data);
int callback_glewlwyd_api_description (const struct _u_request * request, struct _u_response * response, void * user_data);

int callback_default (const struct _u_request * request, struct _u_response * response, void * user_data);

json_t * auth_check(struct config_elements * config, const char * username, const char * password, const char * scope_list);
json_t * auth_check_credentials_database(struct config_elements * config, const char * username, const char * password, const char * scope_list);
json_t * auth_check_credentials_ldap(struct config_elements * config, const char * username, const char * password, const char * scope_list);
json_t * auth_check_scope(struct config_elements * config, const char * username, const char * scope_list);
json_t * auth_check_scope_database(struct config_elements * config, const char * username, const char * scope_list);
json_t * auth_check_scope_ldap(struct config_elements * config, const char * username, const char * scope_list);
json_t * session_check(struct config_elements * config, const struct _u_request * request);
char * generate_access_token(struct config_elements * config, const char * refresh_token, const char * username, const uint auth_type, const char * ip_source, const char * scope_list, time_t now);
char * generate_refresh_token(struct config_elements * config, const char * username, const uint auth_type, const char * ip_source, const char * scope_list, time_t now);
char * generate_session_token(struct config_elements * config, const char * username, const char * ip_source, const char * scope_list, time_t now);

int client_check(struct config_elements * config, uint auth_check, const char * client_id, const char * redirect_uri);
int auth_check_client_user_scope(struct config_elements * config, const char * client_id, const char * username, const char * scope_list);

int serialize_refresh_token(struct config_elements * config, const char * username, const uint auth_type, const char * ip_source, const char * refresh_token, time_t now);
int serialize_access_token(struct config_elements * config, const uint auth_type, const char * ip_source, const char * refresh_token);
int is_authorization_type_enabled(struct config_elements * config, uint authorization_type);

#endif
