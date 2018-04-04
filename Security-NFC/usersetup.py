import pickle
import base64

m = str(input("Zadej e-mail odesilatele: "))
mr = str(input("Zadej e-mail prijemce: "))
x = str(input("Zadej heslo odesilatele: "))
userpass = base64.b64encode(x.encode("utf-8"))
usermail = m
reciever = mr
pickle.dump(userpass, open("psswrd.p", "wb"))
pickle.dump(usermail, open("mail.p", "wb"))
pickle.dump(reciever, open("mailR.p", "wb"))
