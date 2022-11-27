
import pyrebase

config = {
"apiKey": "AIzaSyAWJVYq8o_8DJFNpbl5CMhW28S2Ayyd8DM",
"authDomain": "esp32-aleatorios.firebaseapp.com",
"databaseURL": "https://proyecto1-a433f-default-rtdb.firebaseio.com",
"projectId": "esp32-aleatorios",
"storageBucket": "esp32-aleatorios.appspot.com",
"messagingSenderId": "197506906640",
"appId": "1:197506906640:web:160bd9370c5300a125e895",
"measurementId": "G-GH5PT1E3Z1"
}

firebase = pyrebase.initialize_app(config)
#accesing database in firebase
db = firebase.database()
#reading some atributes of the onKey elementsß
all_users = db.child("users").get()
for users in all_users.each():
    print(users.val())
    print(users.key())
