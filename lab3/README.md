 # Networked Spell Check
 ## Abraham Schultz
 
### This project should implement a basic server.
 
 The server should have a main thread that creates a pool of worker threads.
 
  - the main thread should always be doing two things:
  
  1. Accepting distributing connection requests from clients
  2. Additionally the main thread should be checking to see if there are any requests to write to the log
  these will be added to the log queue and removed as needed.

  The worker threads will continually check if there is anything in the work Queue
  if there is they will run the service routine.
 
 The service routine will check the input against an array of strings and return if there is a match or not.
 if there is a match that means the word is spelled correctly.

 
 ### Files included: 
 - uml-diagram.pdf     // design document
 - netSpell.h          // header file for program
 - netSpell.cpp        // this contains the main while loop
 - Init.cpp            // read in dictionary
 - dictionary.txt      // list of correctly spelled words
 

 