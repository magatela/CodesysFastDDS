# run the script to generate the message code
/home/imig/Downloads/src/fastddsgen/scripts/fastddsgen <file_name>.idl

# to run the publisher and subscriber
1.  cd cd <work_space>/build
2.  cmake ..
3.  make
4.  in two differen terminals
5.  Termianl 1                   |   Terminal 2
    sudo ./DDSCodesysPublisher   |   sudo ./DDSCodesysPublisher
6.  if there is no communication between the nodes it is because they 
    are not using the same environment variable
7.  sudo du    
8.  source <work_space>/setup.bash
9.  make
10  step 5 again without "sudo" 
11  in the Codesys IDE change the value of pointerToData.zynchonizer to four to start reading the data
