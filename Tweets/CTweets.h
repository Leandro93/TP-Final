/*
 * CTweets
 */

#include "include/json/json.h"
#include "include/twitcurl.h"

#define         OK      1
#define         NOT_OK  0

typedef struct Location {
    double longitude;
    double latitude;
} Location;

typedef struct CTweet {
	char *user;
	char *userName;	
	char *text;
	char *lang;	//http://en.wikipedia.org/wiki/IETF_language_tag, or "und" if no language could be detected.
                        //See the list of available languages here: http://www.iana.org/assignments/language-subtag-registry/language-subtag-registry
	Location loc;	//lat = 0, long = 0 if tweet's not geotagged
} CTweet;


int     CT_init            (char *err);

/*
 * For info on how to build a query, check out: https://dev.twitter.com/docs/using-search and 
 * http://en.wikipedia.org/wiki/URL_encoding
 * Please note that Twitter's search service and, by extension, the Search API is not meant 
 * to be an exhaustive source of Tweets. Not all Tweets will be indexed or made available via the
 * search interface.
 * Search will always return new tweets. To go back in time, call searchReset. 
 * It is recommended to do so every time you change you query.
 * Count is the amount of tweets desired. On return, it will have the actual value of tweets fetched.
 * If everything went ok, err = NULL. Else, error message.
 */

CTweet *CT_search          (char *searchQuery, unsigned int *count, char *err);

void    CT_searchReset     (void);

void    CT_freeTweets      (CTweet *buffer);



