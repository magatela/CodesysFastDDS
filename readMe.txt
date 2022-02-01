# run the script to generate the message code
~/Downloads/src/fastddsgen/scripts/fastddsgen <file_name>.idl

# to run the publisher and subscriber
1.  Download the .zip code or clone the repository
2.  go to your_downloads/CodesysFastDDS-development/ and create a new folder "build" 
3.  cd build
4.  cmake ..
5.  make
6.  in two differen terminals
7.  Termianl 1                   |   Terminal 2
    sudo ./DDSCodesysPublisher   |   sudo ./DDSCodesysSubscriber

8.  if there is no communication between the nodes it is because they 
    are not using the same environment variable
9.  sudo su    
10. source <work_space>/setup.bash
11. make
12. step 5 again without "sudo" 
13. in the Codesys IDE change the value of pointerToData.zynchonizer to four to start reading the data
