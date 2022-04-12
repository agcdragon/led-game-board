#!/usr/bin/env python

# simple program to compile and upload Arduino code using the Arduino command line
import subprocess
import sys
    
# Create a Tkinter GUI
from tkinter import *

GAME = "tictactoe"

def id():
    start()

def ai():
    global GAME
    GAME += "_ai"
    start() 

def ttt():
    global GAME
    GAME = "tictactoe"
    for widget in win.winfo_children():
        widget.destroy()
    frame = Frame(win)
    frame.pack()
    Button(frame, text="PvP", font=(f), width=w, height=h*2, command=id).grid(row=0, column=0)
    Button(frame, text="AI", font=(f), width=w, height=h*2, command=ai).grid(row=0, column=1)
    win.mainloop()

def connect4():
    global GAME
    GAME = "connect4"
    frame = Frame(win)
    frame.pack()
    Button(frame, text="PvP", font=(f), width=w, height=h, command=id).grid(row=0, column=0)
    Button(frame, text="AI", font=(f), width=w, height=h, command=ai).grid(row=0, column=1)
    win.mainloop()

def battle():
    global GAME
    GAME = "battleship"
    frame = Frame(win)
    frame.pack()
    Button(frame, text="PvP", font=(f), width=w, height=h, command=id).grid(row=0, column=0)
    Button(frame, text="AI", font=(f), width=w, height=h, command=ai).grid(row=0, column=1)
    win.mainloop()

def snake():
    global GAME
    GAME = "snake"

def reset():
    for widget in win.winfo_children():
        widget.destroy()
    frame = Frame(win)
    frame.pack()
    Button(frame, text="Tic-Tac-Toe", font=(f), width=w, height=h, command=ttt).grid(row=0, column=0)
    Button(frame, text="Connect4", font=(f), width=w, height=h, command=ttt).grid(row=0, column=1)
    Button(frame, text="Battleship", font=(f), width=w, height=h, command=ttt).grid(row=1, column=0)
    Button(frame, text="Snake", font=(f), width=w, height=h, command=ttt).grid(row=1, column=1)
    win.mainloop()

##########################################    
# Credits to @Robin2 on forum.arduino.cc #
##########################################
def start():
    print(GAME)
    actionLine = "upload"
    projectFile = f"C:/Users/andy/ThetaTau/theta-tau-ep/{GAME}/{GAME}.ino"

    codeFile = open(projectFile, 'r')
    startLine = codeFile.readline()[3:].strip()
    arduinoProg = codeFile.readline()[3:].strip()
    boardLine = codeFile.readline()[3:].strip()
    portLine = codeFile.readline()[3:].strip()
    endLine = codeFile.readline()[3:].strip()
    codeFile.close()

    if (startLine != "python-build-start" or endLine != "python-build-end"):
        print("\n Error in build-commands - can't process file")
        sys.exit()

    arduinoCommand = arduinoProg + " --" + actionLine + " --board " + boardLine + " --port " + portLine + " --verbose " + projectFile

    print("\n\n -- Arduino Command --")
    print(arduinoCommand)

    print("-- Starting %s --\n" %(actionLine))

    presult = subprocess.call(arduinoCommand, shell=True)

    if presult != 0:
        print("\n Failed - result code = %s --" %(presult))
    else:
        print("\n-- Success --")

    # Add reset functionalities
    for widget in win.winfo_children():
        widget.destroy()
    frame = Frame(win)
    frame.pack()
    Button(frame, text="Reset", font=(f), width=w*2, height=h*2, command=reset).grid(row=0, column=0)
    win.mainloop()

# Create tkinter object and initialize window
win = Tk()
win.geometry = ("700x480")

w = 20
h = 5
f = "Arial 20"
reset()