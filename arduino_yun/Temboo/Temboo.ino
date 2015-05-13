#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h" // contains Temboo account information, as described below

int numRuns = 1;   // Execution count, so this doesn't run forever
int maxRuns = 10;   // Maximum number of times the Choreo should be executed

void setup() {
  Serial.begin(9600);
  
  // For debugging, wait until the serial console is connected
  delay(4000);
  while(!Serial);
  Bridge.begin();
}

void loop() {
    // while we haven't reached the max number of runs...
  //  if (numRuns <= maxRuns) {
    //Serial.println("Running UserTimeline - Run #" + String(numRuns++));
    
    TembooChoreo UserTimelineChoreo;

    // invoke the Temboo client.
    // NOTE that the client must be reinvoked, and repopulated with
    // appropriate arguments, each time its run() method is called.
    UserTimelineChoreo.begin();

    // Set Temboo account credentials
    UserTimelineChoreo.setAccountName(TEMBOO_ACCOUNT);
    UserTimelineChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    UserTimelineChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // Set Choreo inputs
    UserTimelineChoreo.addInput("ScreenName", ScreenName);
    UserTimelineChoreo.addInput("AccessToken", AccessToken);
    UserTimelineChoreo.addInput("AccessTokenSecret", AccessTokenSecret);
    UserTimelineChoreo.addInput("ConsumerSecret", ConsumerSecret);
    UserTimelineChoreo.addInput("ConsumerKey", ConsumerKey);
    
    // Identify the Choreo to run
    UserTimelineChoreo.setChoreo("/Library/Twitter/Timelines/UserTimeline");
    
    // we want the text of the tweet
    UserTimelineChoreo.addOutputFilter("tweet", "/[1]/text", "Response");
    
    // and the name of the author
    UserTimelineChoreo.addOutputFilter("author", "/[1]/user/screen_name", "Response");
    
    // Run the Choreo; when results are available, print them to serial
    UserTimelineChoreo.run();
   
    String author; // a String to hold the tweet author's name
    String tweet; // a String to hold the text of the tweet

      // choreo outputs are returned as key/value pairs, delimited with 
      // newlines and record/field terminator characters, for example:
      // Name1\n\x1F
      // Value1\n\x1E
      // Name2\n\x1F
      // Value2\n\x1E      
      
    // see the examples at http://www.temboo.com/arduino for more details
    // we can read this format into separate variables, as follows:
    
    while(UserTimelineChoreo.available()) {
        // read the name of the output item
        String name = UserTimelineChoreo.readStringUntil('\x1F');
        name.trim();

        // read the value of the output item
        String data = UserTimelineChoreo.readStringUntil('\x1E');
        data.trim();

        // assign the value to the appropriate String
        if (name == "tweet") {
          tweet = data;
        } else if (name == "author") {
          author = data;
        }
      }
     
    Serial.println("@" + author + " - " + tweet);
    UserTimelineChoreo.close();
  //}

  Serial.println("Waiting...");
  delay(1000); // wait 30 seconds between UserTimeline calls
}
