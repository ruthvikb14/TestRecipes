import tkinter 
import os
from tkinter import *
import tkinter.ttk as ttk
from tkinter.messagebox import *
from tkinter.filedialog import *
import subprocess
import csv
from tkintertable import TableCanvas, TableModel
import pandas as pd
import matplotlib
import matplotlib.pyplot as plt
matplotlib.use("TkAgg")
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure
import warnings
warnings.filterwarnings('ignore')


class chef():
    __root = Tk()
    
    # default window width and height 
    __thisWidth = 200
    __thisHeight = 200
    __thisMenuBar = Menu(__root, relief = FLAT)
    __thisFileMenu = Menu(__thisMenuBar, tearoff=0)
    __text = Text()
    __thisHelpMenu = Menu(__thisMenuBar, tearoff=0)
    __cls = Button()
    __checkFrame1 = Frame()
    __checkFrame2 = Frame()
    __tableFrame = Frame()
    __toolbarFrame = Frame()
    __fig = plt.Figure()
    __file = None

    def __init__(self,**kwargs): 
        
        try: 
            self.__thisWidth = kwargs['w'] 
        except KeyError: 
            pass

        try: 
            self.__thisHeight = kwargs['h'] 
        except KeyError: 
            pass

        # Set the window text 
        self.__root.title("MagOD Chef") 

        # Center the window 
        screenWidth = self.__root.winfo_screenwidth() 
        screenHeight = self.__root.winfo_screenheight() 
        
        # For left-allignment
        left = (screenWidth / 2) - (self.__thisWidth / 2) 

        # For right-allignment
        right = (screenHeight / 2) - (self.__thisHeight /2) 
        
        # For top and bottom 
        self.__root.geometry('%dx%d+%d+%d' % (self.__thisWidth, self.__thisHeight, left, right)) 
        self.__root.resizable(1, 1)
        self.__root.grid()
        
        # Add Frame
        self.__checkFrame1 = Frame(self.__root).grid(row=0,column=0,sticky='NEWS')
        self.__checkFrame2 = Frame(self.__root).grid(row=0,column=8,sticky='NEWS')
        self.__tableFrame = Frame(self.__root).grid(column=1,row=10,columnspan=7,rowspan=6,sticky='NEWS')
        self.__toolbarFrame = Frame(self.__root)
        self.__toolbarFrame.grid(column=2,row=9,rowspan=1,columnspan=4,sticky='NW')
        extra = Frame(self.__root).grid(row=20,column=0,rowspan=2,columnspan=8,sticky='NEWS')
        
        # Add Graph
        self.__fig = plt.Figure(dpi = 100, figsize = (5,3))
        self.__fig.subplots_adjust(left = 0.10,right = 0.9,bottom = 0.15,top = 0.93)
        self.__ax = self.__fig.add_subplot(1,1,1, frameon=True)
        self.__ax.set(xlim=[0,2400],ylim=[-3,3])
        self.__ax.set_xlabel('Time(s)')
        self.__axe = self.__ax.twinx()
        self.__axe.set_ylabel('Led Intensity', color='crimson')
        self.__axe.set(ylim=[0, 255])
        
        self.chart = FigureCanvasTkAgg(self.__fig,self.__root)
        
        self.chart.get_tk_widget().grid(column=1,row=0,rowspan=9,columnspan=7,sticky='NEWS')
        
        self.toolbar = NavigationToolbar2Tk(self.chart, self.__toolbarFrame)
        self.toolbar.update()
        
        # Add variables
        self.__Var1 = BooleanVar()
        self.__Var2 = BooleanVar()
        self.__Var3 = BooleanVar()
        self.__Var4 = BooleanVar()
        self.__Var5 = BooleanVar()
        self.__Var6 = BooleanVar()
        
        # Add CheckButtons (widget)
        self.checkBtn()
        
        # Add Table (widget)
        # self.table()
        self.tree()
        
        # Add Text (widget) 
        self.text()
        
        # To open a already existing file 
        self.__thisFileMenu.add_command(label="Open", command=self.__openFile)
        self.__thisFileMenu.add_command(label="Exit", command=self.__exit)
        self.__thisMenuBar.add_cascade(label="  File  ", menu=self.__thisFileMenu, underline=0)
        
        # To create a feature of description
        self.__thisHelpMenu.add_command(label="About", command=self.__showAbout) 
        self.__thisMenuBar.add_cascade(label="  Help  ", menu=self.__thisHelpMenu, underline=0) 
        
        self.__root.config(menu=self.__thisMenuBar)
        self.config()
        
        mag = Label(extra, text='MagOD Chef', font=("Times", 3, "bold")).grid(column=8, sticky='S')
        self.count = 0
    
    def v_lines(self):
        for i in y:
            self.__ax.axvline(x[i-1], linestyle='--', color='coral') # vertical lines
    
    def graph(self):
        self.__ax.set_xlabel('Time(s)')
        self.__axe.set_ylabel('Led Intensity', color='crimson')
        self.__ax.step(x, y1, color='black', label='B_x (mT)', where='pre')
        self.__ax.step(x, y2, color='black', linestyle='-.', label='B_y (mT)', where='pre')
        self.__ax.step(x, y3, color='black', linestyle=':', label='B_z (mT)', where='pre')
        self.__axe.step(x, y4, color='red', linestyle='--', label='RED', where='pre')
        self.__axe.step(x, y5, color='green', linestyle='--', label='GREEN', where='pre')
        self.__axe.step(x, y6, color='blue', linestyle='--', label='BLUE', where='pre')
        self.__ax.set(ylim=[-3,3])
        self.__axe.set(ylim=[0,255])
        self.v_lines()
        self.__ax.legend(loc='best')
        self.__axe.legend(loc='center right')
        self.chart.draw()
    
    def ch_bx(self):
        self.check()
        self.v_lines()
        if(self.__Var1.get()):
            self.pt1, = self.__ax.step(x, y1, color='black', label='B_x (mT)', where='pre')
            self.__ax.set(xlim=[min(x),max(x)], ylim=[min(y1)-1,max(y1)+1])
            self.chart.draw()
        else:
            self.pt1.set_xdata([0])
            self.pt1.set_ydata([0])
            self.chart.draw()
    
    def ch_by(self):
        self.check()
        self.v_lines()
        if(self.__Var2.get()):
            self.pt2, = self.__ax.step(x, y2, color='black', linestyle='-.', label='B_y (mT)', where='pre')
            self.__ax.set(xlim=[min(x),max(x)], ylim=[min(y2)-1,max(y2)+1])
            self.chart.draw()
        else:
            self.pt2.set_xdata([0])
            self.pt2.set_ydata([0])
            self.chart.draw()
    
    def ch_bz(self):
        self.check()
        self.v_lines()
        if(self.__Var3.get()):
            self.pt3, = self.__ax.step(x, y3, color='black', linestyle=':', label='B_z (mT)', where='pre')
            self.__ax.set(xlim=[min(x),max(x)], ylim=[min(y3)-1,max(y3)+1])
            self.chart.draw()
        else:
            self.pt3.set_xdata([0])
            self.pt3.set_ydata([0])
            self.chart.draw()
    
    def ch_red(self):
        self.check()
        self.v_lines()
        if(self.__Var4.get()):
            self.pt4, = self.__axe.step(x, y4, color='red', linestyle='--', label='RED', where='pre')
            self.__axe.set(xlim=[min(x),max(x)], ylim=[min(y4), max(y4)+10])
            self.chart.draw()
        else:
            self.pt4.set_xdata([0])
            self.pt4.set_ydata([0])
            self.chart.draw()
    
    def ch_green(self):
        self.check()
        self.v_lines()
        if(self.__Var5.get()):
            self.pt5, = self.__axe.step(x, y5, color='green', linestyle='--', label='GREEN', where='pre')
            self.__axe.set(xlim=[min(x),max(x)], ylim=[min(y5), max(y5)+10])
            self.chart.draw()
        else:
            self.pt5.set_xdata([0])
            self.pt5.set_ydata([0])
            self.chart.draw()
    
    def ch_blue(self):
        self.check()
        self.v_lines()
        if(self.__Var6.get()):
            self.pt6, = self.__axe.step(x, y6, color='blue', linestyle='--', label='BLUE', where='pre')
            #self.__axe.set(xlim=[min(x),max(x)], ylim=[min(y6), max(y6)+10])
            self.chart.draw()
        else:
            self.pt6.set_xdata([0])
            self.pt6.set_ydata([0])
            self.chart.draw()
    
    def check(self):
        if((self.__Var1.get() or self.__Var2.get() or self.__Var3.get() or self.__Var4.get() or self.__Var5.get() or self.__Var6.get()) and self.count==0):
            self.count=1
            self.__ax.cla()
            self.__axe.cla()
            self.__ax.set_xlabel('Time(s)')
            self.__axe.set_ylabel('Led Intensity', color='crimson')
            self.__ax.set(ylim=[-3,3])
            self.__axe.set(ylim=[0,255])
            
    
    def checkBtn(self):
        _Bx = Checkbutton(self.__checkFrame1, text = "Bx", padx=5, pady=5, variable = self.__Var1,
                font=("Times", 10, "bold"), command=self.ch_bx).grid(row=0,sticky='N')

        _By = Checkbutton(self.__checkFrame1, text = "By", padx=5, pady=5, variable = self.__Var2, 
                font=("Times", 10, "bold"), command=self.ch_by).grid(row=1,sticky='N')

        _Bz = Checkbutton(self.__checkFrame1, text = "Bz", padx=5, pady=5, variable = self.__Var3, 
                font=("Times", 10, "bold"), command=self.ch_bz).grid(row=2,sticky='N')
        
        _red = Checkbutton(self.__checkFrame2, text = " RED   ", padx=5, pady=5, variable = self.__Var4, fg = "red",
                            font=("Times", 10, "bold"), command=self.ch_red)
        _red.grid(column=8,row=0,sticky='N')

        _green = Checkbutton(self.__checkFrame2, text = "GREEN", padx=5, variable = self.__Var5, fg = "green",
                            font=("Times", 10, "bold"), command=self.ch_green)
        _green.grid(column=8,row=1,sticky='N')
        
        _blue = Checkbutton(self.__checkFrame2, text = " BLUE ", padx=5, pady=5, variable = self.__Var6, fg = "blue",
                            font=("Times", 10, "bold"), command=self.ch_blue)
        _blue.grid(column=8,row=2,sticky='N')
    
    # def table(self):
        # f = Canvas(self.__root).grid(column=1,row=10,columnspan=7,rowspan=6,sticky='NEWS')
        # frame1= Frame(f).grid(sticky='NEWS')
        # mod = TableModel(rows=30, columns=6)
        # self.__table = TableCanvas(parent=frame1, model=mod,cellwidth=83, 
                            # thefont=('Arial',8),rowheight=35,rowheaderwidth=30,editable=True)
        # self.__table.createTableFrame()

    def tree(self):
        style = ttk.Style()
        style.configure("mystyle.Treeview.Heading", font=('Calibri', 10,'italic','bold')) # Modify the font of the headings
        style.layout("mystyle.Treeview", [('mystyle.Treeview.treearea', {'sticky': 'nswe'})]) # Remove the borders
        scrollbar_y = Scrollbar(self.__tableFrame, orient=VERTICAL)
        self.__tree = ttk.Treeview(self.__tableFrame, 
                                   columns=("sno", "bx", "by", "bz", "l_t", "l_c", "l_i"), style="mystyle.Treeview",
                                   height=10, selectmode="browse", yscrollcommand=scrollbar_y.set, padding = 2)
        scrollbar_y.config(command=self.__tree.yview)
        scrollbar_y.grid(self.__tableFrame, column=1, row=10, rowspan=6, sticky='NEWS')
        self.__tree.heading('sno', text="Step")
        self.__tree.heading('bx', text="B_x (mT)")
        self.__tree.heading('by', text="B_y (mT)")
        self.__tree.heading('bz', text="B_z (mT)")
        self.__tree.heading('l_t', text="Led Time (ms)")
        self.__tree.heading('l_c', text="Led Color")
        self.__tree.heading('l_i', text="Led Intensity (0-255)")
        self.__tree.column('#0', stretch=False, minwidth=0, width=0)
        self.__tree.column('#1', stretch=False, minwidth=0, width=70)
        self.__tree.column('#2', stretch=False, minwidth=0, width=70)
        self.__tree.column('#3', stretch=False, minwidth=0, width=70)
        self.__tree.column('#4', stretch=False, minwidth=0, width=70)
        self.__tree.column('#5', stretch=False, minwidth=0, width=90)
        self.__tree.column('#6', stretch=False, minwidth=0, width=120)
        self.__tree.grid(self.__tableFrame, column=2, row=10, rowspan=6, columnspan=5, sticky='NEWS')
        
    
    def text(self):
        self.__text = Text(self.__root, height = 2, width = 70, xscrollcommand = set(), yscrollcommand = set())
        self.__text.grid(column=1,row=18,columnspan=4,sticky='NW')
        
        self.__cls = Button(self.__root, text = "Clear", padx=15, pady=5, 
                    font=("Times", 10, "bold"), bg = "black", fg = "white", command = self.__clr)
        self.__cls.grid(column=7,row=18, columnspan=2,sticky='NW')
    
    def config(self):
        self.__root.grid_columnconfigure(3,weight=1)
        self.__root.grid_columnconfigure(5,weight=1)
        self.__root.grid_columnconfigure(7,weight=1)
        self.__root.grid_rowconfigure(10,weight=1)
        self.__root.grid_rowconfigure(13,weight=1)
        self.__root.grid_rowconfigure(15,weight=1)
    
    def __clr(self):
        self.__text.delete(1.0, END)
        self.__tree.delete(*self.__tree.get_children())

    def __showAbout(self): 
        showinfo("MagOD Chef","Version 2.0") 

    def __exit(self):
        self.__root.quit()
        self.__root.destroy()

    def __openFile(self):
        
        self.__file = askopenfilename(defaultextension=".csv", filetypes=[("CSV Files","*.csv"), ("All Files","*.*")])
        fname = os.path.basename(self.__file)
        
        if self.__file == "": 
            # no file to open 
            self.__file = None
        else:
            # Try to open the file  
            self.__root.title(os.path.basename(self.__file))
            self.__text.delete(1.0,END) 
            self.__tree.delete(*self.__tree.get_children())
            self.__ax.cla()
            self.__axe.cla()
            
            w = subprocess.Popen(['TestRecipes','-o',fname],stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True)
            output, errors = w.communicate()
            self.__text.insert(END, output)
            
            # For updating values in Tree widget
            with open('temp.csv') as f:
                reader = csv.DictReader(f, delimiter=',')
                for row in reader:
                    sno = row['Step']
                    bx = row['B x']
                    by = row['B y']
                    bz = row['B z']
                    l_t = row['Led Time']
                    l_c = row['Led Color']
                    l_i = row['Led Intensity']
                    self.__tree.insert("", 'end', values=(sno, bx, by, bz, l_t, l_c, l_i))
                
            data = pd.read_csv("temp.csv")
            df = pd.DataFrame(data)
            global x,y,y1,y2,y3,y4,y5,y6
            
            freq = df.groupby(['Recipe Number']).size()
            y = [0]*len(freq)
            for i,j in enumerate(freq):
                y[i] = y[i-1]+j
                
            y1 = df['B x']
            y2 = df['B y']
            y3 = df['B z']
            y4,y5,y6 = self.led_val(df['Led Color'],df['Led Intensity'])
            
            x=[0]*len(df['Led Time'])
            for i,j in enumerate(df['Led Time']):
                x[i]=x[i-1]+j/1000
            plt.ion()
            self.graph()            

    def led_val(self, val, _int):
        _green=[0]*len(val)
        _red=[0]*len(val)
        _blue=[0]*len(val)
        for i,j in enumerate(val):
            if(j==' GREEN'):
                _green[i]=_int[i]
            elif(j==' RED'):
                _red[i]=_int[i]
            elif(j==' BLUE'):
                _blue[i]=_int[i]
            else:
                _green[i]=0
                _red[i]=0
                _blue[i]=0
        return _red,_green,_blue

    def execute(self): 
    # Run main application 
        self.__root.mainloop() 


# Run main application 
win = chef(w=800,h=700) 
win.execute() 