import pickle
import base64

m = str(input("Zadej e-mail: "))
x = str(input("Zadej heslo: "))
userpass = base64.b64encode(x.encode("utf-8"))
usermail = m
pickle.dump(userpass, open("psswrd.p", "wb"))
pickle.dump(usermail, open("mail.p", "wb"))
