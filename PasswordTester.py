import string
import hashlib
import requests
import tkinter as tk

def hash_password(password):
    return hashlib.sha1(password.encode('utf-8')).hexdigest().upper()

def pwned_api_check(password):
    hashed_password = hash_password(password)
    first5, tail = hashed_password[:5], hashed_password[5:]
    
    url = f'https://api.pwnedpasswords.com/range/{first5}'
    res = requests.get(url)

    if res.status_code != 200:
        raise RuntimeError(f'API request error: {res.status_code}')

    hashes = (line.split(':') for line in res.text.splitlines())
    for h, count in hashes:
        if h == tail:
            return int(count)  # password found, returns number of leaks
    return 0  # password not found

def password_strength(password):
    if len(password) < 8:
        return "Weak"
    complexity = sum([
        any(c.islower() for c in password),
        any(c.isupper() for c in password),
        any(c.isdigit() for c in password),
        any(c in string.punctuation for c in password)
    ])
    
    if len(password) >= 12 and complexity >= 3:
        return "Strong"
    elif len(password) >= 8 and complexity >= 2:
        return "Moderate"
    else:
        return "Weak"

#This is all going to be for the GUI
def check_password():
    password = password_entry.get()
    strength = password_strength(password)
    leak_count = pwned_api_check(password)
    
    result = f"Strength: {strength}\n"
    if leak_count:
        result += f"⚠️ Leaked {leak_count} times! Choose another."
    else:
        result += "✅ No leaks found!"
    result_label.config(text=result)

#Just going to create the main window here
root = tk.Tk()
root.title("Password Strength Checker")

#Lets customize the window to be a decent size
root.geometry("400x200")

#Some form of a text box to take the password in
password_entry = tk.Entry(root, width=30, show="*")
password_entry.pack(pady=10)

# Button (to trigger checking)
check_button = tk.Button(root, text="Check Password", command=check_password, font=("Arial", 12))
check_button.pack(pady=10)

# Label (to display results)
result_label = tk.Label(root, text="")
result_label.pack(pady=10)

#Loop to keep the application running
root.mainloop()