import Detect_Voice
import threading
import re
import speech_recognition as sr
import socket
import time

# Note: need to start bot then python script
UDP_IP = "192.168.2.32"
UDP_PORT = 2390
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
bot_running = False



def convert_time(input):
    parts = input.split(':')
    ms = int(parts[3])
    s = int(parts[2])
    min = int(parts[1])
    hr = int(parts[0])
    return str(ms + 1000*s + 60000*min + 3600000*hr)


# this is called from the background thread
def callback(recognizer, audio):
    # received audio data, now we'll recognize it using Google Speech Recognition
    try:
        # for testing purposes, we're just using the default API key
        # to use another API key, use `r.recognize_google(audio, key="GOOGLE_SPEECH_RECOGNITION_API_KEY")`
        # instead of `r.recognize_google(audio)`
        print("Google Speech Recognition thinks you said: " + recognizer.recognize_google(audio))
        matches = re.search('(spin|walk|stay|fetch)',recognizer.recognize_google(audio))
        print(matches)
        if (matches != None):
            if (matches.group(0) == 'spin'):
                    print(last_recv_time)
                    print(convert_time(last_recv_time))
                    sock.sendto(bytes(convert_time(last_recv_time)+",3", "utf-8"), (UDP_IP, UDP_PORT))


# this code shiesty af, sorry to whoever is looking at it
# basically just trusting that the bot is in the right state, no checks
        
        
    except sr.UnknownValueError:
        print("Google Speech Recognition could not understand audio")
    except sr.RequestError as e:
        print("Could not request results from Google Speech Recognition service; {0}".format(e))

r = sr.Recognizer()
m = sr.Microphone()
with m as source:
    r.adjust_for_ambient_noise(source)  # we only need to calibrate once, before we start listening

# start listening in the background (note that we don't have to do this inside a `with` statement)
stop_listening = r.listen_in_background(m, callback)
# `stop_listening` is now a function that, when called, stops background listening

# send stop at start of loop so packets can be received for timestamps
sock.sendto(bytes("0,2", "utf-8"), (UDP_IP, UDP_PORT))
while (True):
    recv = str(sock.recv(256), "utf-8")
    split_str = recv.split(",")
    last_recv_time = split_str[0]

    if (bot_running==True):
        t1 = threading.Thread(target=Detect_Voice.detect_voice)
        t1.start()
        # This represents other functions occuring in python while bot is running
        while(Detect_Voice.global_detect != 1):
            # this control flow is brain damaged
            # put camera processing here
            recv = str(sock.recv(256), "utf-8")
            #print(recv)

        # parse time from last packet received
        split_str = recv.split(",")
        last_recv_time = split_str[0]
        sock.sendto(bytes(convert_time(last_recv_time)+",2", "utf-8"), (UDP_IP, UDP_PORT))
        print("stopped bot")
        bot_running = False

       

        
    


