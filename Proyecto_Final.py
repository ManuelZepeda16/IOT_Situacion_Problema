
import pyrebase #Manejo de firebase con python

config = { #Configuracion de la firebase
"apiKey" : "AIzaSyAWJVYq8o_8DJFNpbl5CMhW28S2Ayyd8DM", 
"authDomain" : "proyecto1-a433f.firebaseapp.com",
"databaseURL": "https://proyecto1-a433f-default-rtdb.firebaseio.com",
"projectId" :"proyecto1-a433f",
"storageBucket" : "proyecto1-a433f.appspot.com",
"messagingSenderId" : "536267312725",
"appId" : "1:536267312725:web:4ba32ddfb6e76a14658edc",
"measurementId" :  "G-B4YEYPY2BW"
}

firebase = pyrebase.initialize_app(config) #inicializamos con los identificadores 
db = firebase.database() #acceso a la base de datos
info = db.child("test").get() #entreamos al bucket donde estan guardados los elementos

for item in info.each(): #for que recorre el bucket e imprime los elementos dentro de el
    if(item.key()=="usuario"):
        print(f'{item.key()} = {item.val()}')
    elif(item.key()=="numero"): 
        if(int(item.val())>9):
            db.child("test").update({"usuario":"elemento mayor a 9"}) #cambio de usuario dentro de la base de datos
            print("Numero mayor que 9")
        else:
            print(f'Numero = {item.val()}')    
    elif("Temperatura C" == item.key()): 
        print(f'{item.key()} = {item.val()}°C')
    elif("TemperaturaF" == item.key()):
        print(f'{item.key()} = {item.val()}°F')
    elif("humedad" == item.key()): 
        print(f'{item.key()} = {item.val()}')
    elif("Flama" == item.key()): 
        if(item.val()==False):
            print("No hay presencia de flama")
        else:
            print("Hay presencia de flama")

    elif("JoyStick_Button" == item.key()): 
        if(item.val()==False):
            print("Estas presionando el boton")
        else:
            print("No estas presionando el boton")
    
    elif("Presion" == item.key()):  
        print(f'{item.key()} = {item.val()}')
