#include <bcon.h>
#include <bson.h>
#include <mongoc.h>
#include <stdio.h>
void print_res (const bson_t *reply)
{
   BSON_ASSERT (reply);
   char *str = bson_as_json (reply, NULL);
   printf ("%s\n", str);
   bson_free (str);
}
int main (int argc, char *argv[])
{
   bson_t reply;
   bson_t *command;
   bool res;
   bson_error_t error;
   mongoc_cursor_t *cursor;
   const bson_t *doc;

   bool map_reduce_done = false;
   bool query_done = false;

   const char *out_collection_name = "cspotid_Collection";
   mongoc_collection_t *out_collection;

   /* Empty find query */
   bson_t find_query = BSON_INITIALIZER;

   /* Construct the mapReduce command */

   /* Other arguments can also be specified here, like "query" or
      "limit" and so on */
    const char *const MAPPER2 = "function () {"
                            //"for (var key in this) {"
                            "for (var key in this.content.province_map.yiyiling.spot_ids) {"
                            "emit(key, null);"
                            "};"
                            "}";
    const char *const REDUCER2 = "function (key, values) {"
                                "return null;"
                                "}";
   mongoc_init ();
   mongoc_client_t* client = mongoc_client_new("mongodb://yourip:port");
   mongoc_database_t *database = mongoc_client_get_database (client, "logistics");
   //mongoc_collection_t* collection = mongoc_client_get_collection (client, "logistics", "testcrud");

   command = BCON_NEW ("mapReduce",
                       //BCON_UTF8 ("testcrud"),
                       BCON_UTF8 ("testcrud"),
                       "map",
                       BCON_CODE (MAPPER2),
                       "reduce",
                       BCON_CODE (REDUCER2),
                       "out",
                       BCON_UTF8 (out_collection_name),
                       "query",
                       "{",
                       "table_name", BCON_UTF8("eastchina"),
                       "}"
                       );
   res =
      mongoc_database_command_simple (database, command, NULL, &reply, &error);
   map_reduce_done = true;

   if (!res) {
      fprintf (stderr, "MapReduce failed: %s\n", error.message);
      goto cleanup;
   }

   /* Do something with the reply (it doesn't contain the mapReduce results) */
   print_res (&reply);

   /* Now we'll query outCollection to see what the results are */
   out_collection =
      mongoc_database_get_collection (database, out_collection_name);
   cursor = mongoc_collection_find_with_opts (
      out_collection, &find_query, NULL, NULL);
   query_done = true;

   /* Do something with the results */
   while (mongoc_cursor_next (cursor, &doc)) {
      print_res (doc);
   }

   if (mongoc_cursor_error (cursor, &error)) {
      fprintf (stderr, "ERROR: %s\n", error.message);
      res = false;
      goto cleanup;
   }

cleanup:
   /* cleanup */
   if (query_done) {
      mongoc_cursor_destroy (cursor);
      mongoc_collection_destroy (out_collection);
   }

   if (map_reduce_done) {
      bson_destroy (&reply);
      bson_destroy (command);
   }
   //mongoc_collection_destroy (collection);
   mongoc_database_destroy (database);
   mongoc_client_destroy (client);
   mongoc_cleanup ();
   printf("res is: %d\n", res);
   return 0;
}
