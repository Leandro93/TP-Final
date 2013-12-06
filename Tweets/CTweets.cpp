/*
 * CTweets
 */


#include <iostream>
#include <cstdlib>
#include <string>
#include "CTweets.h"

#define _DEBUG_TWITTER_API      0
#define _DEBUG_JSON             0
#define _DEBUG_CPP              0

using namespace std;

twitCurl twit;
string minId = "0";
string maxId;

char *string2c(string str, char *cstr);

int CT_init(char *err) {

	int ret;
	string resp;
        err = NULL;
        
	/*
	twit.setProxyServerIp(string("ip.ip.ip.ip"));
	twit.setProxyServerPort(string("port"));
	*/
        
        if (twit.isCurlInit() == false){
            ret = NOT_OK;
            err = string2c("Couldn't initialize connection" ,err);
        }
        else {
            //OAuth
            twit.getOAuth().setConsumerKey(string("rHZzY8uIgjcL1KRSyvSQ"));
            twit.getOAuth().setConsumerSecret(string("rAmFK4M7iRCnz7hy8o1OK3ULxRRK6g4eO2sgoZjsk"));
            twit.getOAuth().setOAuthTokenKey(string("161100382-1461j50PpOoWc0xUajl85ZW8ADGroz1Re71bYhbn"));
            twit.getOAuth().setOAuthTokenSecret(string("7iPIBwEaGxoCb8GdG3HBC60GCEhSF3NDuSoisL9K8h1ui"));
            //twit.setTwitterApiType( twitCurlTypes::eTwitCurlApiFormatJson );

            if(twit.accountVerifyCredGet() )
                    ret = OK;
            else {
                    ret = NOT_OK;
                    twit.getLastCurlError(resp);
                    err = string2c(resp, err);
            }
        }
        return ret;
}
//https://dev.twitter.com/docs/using-search
//https://dev.twitter.com/docs/working-with-timelines


CTweet *CT_search(char *searchQuery, unsigned int *count, char *err){

        CTweet *buffer;
    
        string resp;
	string stringQuery = string(searchQuery);
	string stringCount = to_string((int)(*count));

        if(err){}
                //delete[] err;

        stringQuery.append("&since_id=");
        stringQuery.append(minId);
        
        //stringQuery.append("&geocode=37.781157,-122.398720,1000mi");
        
        #if _DEBUG_TWITTER_API
        cout << "searching: " << stringQuery << endl;
        #endif

	if(twit.search(stringQuery, stringCount)){
		twit.getLastWebResponse(resp);

                #if _DEBUG_TWITTER_API
                cout << resp << endl;
                #endif
                
 		Json::Value root;
		Json::Reader json;

		bool parsed = json.parse(resp, root, false);
		if(!parsed)
		{
			//cout << json.getFormatedErrorMessages() << endl;
                        err = string2c(json.getFormatedErrorMessages(), err);
		}
		else {
			const Json::Value results  = root["statuses"];
                        const Json::Value metadata = root["search_metadata"];
                        #if _DEBUG_JSON
                        cout << results.size() << endl;
                        #endif

                        if(results.size()){
                        
                            CTweet *t = new CTweet [results.size()];

                            for(int i=0; i<results.size(); i++)
                            {       //ver asCString
                                    
                                    t[i].user     = string2c(results[i]["id_str"].asString(), t[i].user);
                                    t[i].userName = string2c(results[i]["user"]["screen_name"].asString(), t[i].userName);
                                    t[i].text     = string2c(results[i]["text"].asString(), t[i].text);
                                    
                                    if(results[i]["user"]["lang"].isNull())
                                        t[i].lang = NULL;
                                    else
                                        t[i].lang     = string2c(results[i]["user"]["lang"].asString(), t[i].lang);
                                    
                                    
                                    
                                    if(results[i]["geo"].isNull()){
                                        t[i].loc.latitude  = 0;
                                        t[i].loc.longitude = 0;
                                    }
                                    else {
                                        if (results[i]["geo"]["coordinates"].isArray()){                                    
                                            t[i].loc.latitude  = results[i]["geo"]["coordinates"][0u].asDouble();
                                            t[i].loc.longitude = results[i]["geo"]["coordinates"][1u].asDouble();
                                        }
                                    }

                            }
                            buffer = t;
                            minId = metadata["max_id_str"].asString();
                        }
                        else
                            buffer = NULL;
                        
                        *count = results.size();
                        
                        #if _DEBUG_CPP
                        cout << buffer[0].text << endl;                                                                                                                                                                                                                                                                                                                                                                                   
                        cout << buffer[0].lang << endl;
                        #endif  
		}
	}

	else {
		twit.getLastCurlError( resp );
                //cout << err << endl;
		err = string2c(resp, err);
	}
        
        #if _DEBUG_CPP
        cout << buffer[0].lang << endl;
        #endif
	
        return buffer;
}

void CT_searchReset(void){
    minId = "0";
}

void CT_freeTweets(CTweet *buffer){
    delete[] buffer;
}

char *string2c(string str, char *cstr){
    
	if(str.empty())
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
