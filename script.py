import paho.mqtt.client as mqtt_client
import subprocess
import shlex

def on_connect(client,userdata,flags,rc): 
	if rc==0:
		print("client is connected")
	else:
		print("client is not connected")
def on_message(client, userdata, message):
	print("Message received " + str(message.payload.decode("utf-8")))
	print("Topic "+str(message.topic))
	if len(message.payload)>10:
		subprocess.call(shlex.split(f"./script.sh {message.payload}{message.topic}"))
broker_address="192.168.0.17"
port=1883
client=mqtt_client.Client(mqtt_client.CallbackAPIVersion.VERSION1,"MQTT")
client.on_message=on_message
client.on_connect=on_connect
client.connect(broker_address,port=port)
topic = [("station1",0),("station2",0)]
client.subscribe(topic)
client.loop_start()
while True:
	pass
    
    
    
    