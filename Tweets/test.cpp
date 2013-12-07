/*
 * CTweets
 */


#include <iostream>
#include <stdlib>
#include <string>
#include "CTweets.h"

twitCurl twit;
typedef char *CT_error;
CT_error err;
string resp;
string maxId;


int CT_init() {
{
	int ret;
	
	/*
	twit.setProxyServerIp(string("ip.ip.ip.ip"));
	twit.setProxyServerPort(string("port"));
	*/

	//OAuth
	twit.getOAuth().setConsumerKey(string("rHZzY8uIgjcL1KRSyvSQ"));
	twit.getOAuth().setConsumerSecret(string("rAmFK4M7iRCnz7hy8o1OK3ULxRRK6g4eO2sgoZjsk"));
	twit.getOAuth().setOAuthTokenKey(string("161100382-1461j50PpOoWc0xUajl85ZW8ADGroz1Re71bYhbn"));
	twit.getOAuth().setOAuthTokenSecret(string("7iPIBwEaGxoCb8GdG3HBC60GCEhSF3NDuSoisL9K8h1ui"));
	twit.setTwitterApiType( twitCurlTypes::eTwitCurlApiFormatJson );
	
	if(twit.accountVerifyCredGet() )
		ret = OK;
	else {
		ret = NOT_OK;
		twit.getLastCurlError( resp );
		//err = resp.c_string();
	}
}

int CT_search(char *searchQuery, CTweet **buffer, unsigned int count){

	string stringQuery = string(searchQuery);
	string stringCount = to_string(count);

	if(twit.search( &stringQuery, stringCount){
		twit.getLastWebResponse(resp);

		Json::Value root;
		Json::Reader json;

		bool parsed = json.parse(resp, root, false);
		if(!parsed)
		{
			console() << json.getFormattedErrorMessages() << std::endl;
		}
		else
		{
			const Json::Value results = root["results"];
			
			new CTweet t[results.size];

			for(int i=0; i<results.size(); i++)
			{
				t[i].user 	= string2c(results[i]["id_str"].asString(), t[i].user);
				//t[i].userName 	= string2c(results[i]["id_str"].asString(), t[i].user);
				t[i].text	= string2c(results[i]["text"].asString(), t[i].user);
				t[i].lang	= string2c(results[i]["id_str"].asString(), t[i].user);

			}

			buffer = &t;
		}

	}

	else {
		twit.getLastCurlError( resp );
		delete err;
		string2c(resp, err);
	}
	

}

char *string2c(string str, char *cstr){
	if(str == NULL)
		cstr = NULL;
	else {
		cstr = new char [str.length()+1];
		strcpy (cstr, str.c_str());
	}
	return cstr;
}


    /* Twitter trends APIs (JSON) 
bool trendsGet();
    bool trendsDailyGet();
    bool trendsWeeklyGet();
    bool trendsCurrentGet();
    bool trendsAvailableGet();

*/
