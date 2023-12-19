import speech_recognition as sr
sr.__version__
r = sr.Recognizer()
print(sr.Microphone.list_microphone_names())
mic = sr.Microphone()
with mic as source:
    r.adjust_for_ambient_noise(source)
    audio = r.listen(source)

print(r.recognize_google(audio))