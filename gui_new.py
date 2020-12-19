import tkinter 
import os
from tkinter import *
import tkinter.ttk as ttk
from tkinter.messagebox import *
from tkinter.filedialog import *
import subprocess
from tkintertable import TableCanvas, TableModel
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
        self.__tableFrame = Frame(self.__root)
        self.__tableFrame.grid(column=1,row=10,columnspan=7,rowspan=6,sticky='NEWS')
        self.__toolbarFrame = Frame(self.__root)
        self.__toolbarFrame.grid(column=1,row=9,rowspan=1,columnspan=2,sticky='NW')
        extra = Frame(self.__root).grid(row=20,column=0,rowspan=2,columnspan=8,sticky='NW')
        
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
        self._bx = None
        self._by = None
        self._bz = None
        self._red = None
        self._green = None
        self._blue = None
        
        # Add Table (widget)
        self.table()
        self.__root.bind("<Button-1>",self.update_event)
        
        # Add Text (widget) 
        self.text()
        
        # To open a already existing file 
        self.__thisFileMenu.add_command(label="Open", command=self.__openFile)
        self.__thisFileMenu.add_separator()
        self.__thisFileMenu.add_command(label="Exit", command=self.__exit)
        self.__thisMenuBar.add_cascade(label="  File  ", menu=self.__thisFileMenu, underline=0)
        
        # To create a feature of description
        self.__thisHelpMenu.add_command(label="About", command=self.__showAbout) 
        self.__thisMenuBar.add_cascade(label="  Help  ", menu=self.__thisHelpMenu, underline=0) 
        
        self.__root.config(menu=self.__thisMenuBar)
        self.config()
        
        mag = Label(extra, text='MagOD Chef', font=("Arial", 3, "bold")).grid(column=8, sticky='S')
        
        self.x = None
        self.y = None
        self.y1 = None
        self.y2 = None
        self.y3 = None
        self.y4 = None
        self.y5 = None
        self.y6 = None
    
    def v_lines(self):
        for i in self.y:
            self.__ax.axvline(self.x[i-1], linestyle='-.', color='silver') # vertical lines
    
    def graph(self):
        self.__ax.set_xlabel('Time(s)')
        self.__axe.set_ylabel('Led Intensity', color='crimson')
        self.__ax.step(self.x, self.y1, color='black', linestyle='-', label='B_x (mT)', where='pre')
        self.__ax.step(self.x, self.y2, color='tomato', linestyle='--', label='B_y (mT)', where='pre')
        self.__ax.step(self.x, self.y3, color='chocolate', linestyle=':', label='B_z (mT)', where='pre')
        self.__axe.step(self.x, self.y4, color='red', linestyle='-', label='RED', where='pre')
        self.__axe.step(self.x, self.y5, color='green', linestyle='--', label='GREEN', where='pre')
        self.__axe.step(self.x, self.y6, color='blue', linestyle=':', label='BLUE', where='pre')
        self.__ax.set(ylim=[-3,3])
        self.__axe.set(ylim=[0,255])
        self.v_lines()
        self.__ax.legend(loc='best')
        self.__axe.legend(loc='lower right')
        self.chart.draw()
    
    def ch_bx(self):
        self.check()
        self.v_lines()
        if(self.__Var1.get()):
            self.pt1, = self.__ax.step(self.x, self.y1, color='black', label='B_x (mT)', where='pre')
            self.chart.draw()
        else:
            self.pt1.set_xdata([0])
            self.pt1.set_ydata([0])
            self.chart.draw()
    
    def ch_by(self):
        self.check()
        self.v_lines()
        if(self.__Var2.get()):
            self.pt2, = self.__ax.step(self.x, self.y2, color='tomato', linestyle='--', label='B_y (mT)', where='pre')
            self.chart.draw()
        else:
            self.pt2.set_xdata([0])
            self.pt2.set_ydata([0])
            self.chart.draw()
    
    def ch_bz(self):
        self.check()
        self.v_lines()
        if(self.__Var3.get()):
            self.pt3, = self.__ax.step(self.x, self.y3, color='chocolate', linestyle=':', label='B_z (mT)', where='pre')
            self.chart.draw()
        else:
            self.pt3.set_xdata([0])
            self.pt3.set_ydata([0])
            self.chart.draw()
    
    def ch_red(self):
        self.check()
        self.v_lines()
        if(self.__Var4.get()):
            self.pt4, = self.__axe.step(self.x, self.y4, color='red', label='RED', where='pre')
            self.chart.draw()
        else:
            self.pt4.set_xdata([0])
            self.pt4.set_ydata([0])
            self.chart.draw()
    
    def ch_green(self):
        self.check()
        self.v_lines()
        if(self.__Var5.get()):
            self.pt5, = self.__axe.step(self.x, self.y5, color='green', linestyle='--', label='GREEN', where='pre')
            self.chart.draw()
        else:
            self.pt5.set_xdata([0])
            self.pt5.set_ydata([0])
            self.chart.draw()
    
    def ch_blue(self):
        self.check()
        self.v_lines()
        if(self.__Var6.get()):
            self.pt6, = self.__axe.step(self.x, self.y6, color='blue', linestyle=':', label='BLUE', where='pre')
            self.chart.draw()
        else:
            self.pt6.set_xdata([0])
            self.pt6.set_ydata([0])
            self.chart.draw()
    
    def check(self):
        if((self.__Var1.get() or self.__Var2.get() or self.__Var3.get() or 
            self.__Var4.get() or self.__Var5.get() or self.__Var6.get()) and self.count):
            self.count = False
            self.__ax.cla()
            self.__axe.cla()
            self.__ax.set_xlabel('Time(s)')
            self.__axe.set_ylabel('Led Intensity', color='crimson')
            self.__ax.set(ylim=[-3,3])
            self.__axe.set(ylim=[0,255])
    
    def checkBtn(self):
        self._bx = Checkbutton(self.__checkFrame1, text = "Bx", padx=5, pady=5, variable = self.__Var1, fg = "black",
                font=("Arial", 10, "bold"), command=self.ch_bx).grid(row=0,sticky='N')

        self._by = Checkbutton(self.__checkFrame1, text = "By", padx=5, pady=5, variable = self.__Var2, fg = "tomato", 
                font=("Arial", 10, "bold"), command=self.ch_by).grid(row=1,sticky='N')

        self._bz = Checkbutton(self.__checkFrame1, text = "Bz", padx=5, pady=5, variable = self.__Var3, fg = "saddle brown", 
                font=("Arial", 10, "bold"), command=self.ch_bz).grid(row=2,sticky='N')
        
        self._red = Checkbutton(self.__checkFrame2, text = " RED   ", padx=5, pady=5, variable = self.__Var4, fg = "red",
                            font=("Arial", 10, "bold"), command=self.ch_red)
        self._red.grid(column=8,row=0,sticky='N')

        self._green = Checkbutton(self.__checkFrame2, text = "GREEN", padx=5, variable = self.__Var5, fg = "green",
                            font=("Arial", 10, "bold"), command=self.ch_green)
        self._green.grid(column=8,row=1,sticky='N')
        
        self._blue = Checkbutton(self.__checkFrame2, text = " BLUE ", padx=5, pady=5, variable = self.__Var6, fg = "blue",
                            font=("Arial", 10, "bold"), command=self.ch_blue)
        self._blue.grid(column=8,row=2,sticky='N')
    
    def table(self):
        mod = TableModel(rows=30, columns=6)
        self.__table = pt = TableCanvas(self.__tableFrame, model=mod)
        pt.show()
    
    def text(self):
        self.__text = Text(self.__root, height = 2, width = 70, xscrollcommand = set(), yscrollcommand = set())
        self.__text.grid(column=1,row=18,columnspan=4,sticky='NW')
        
        self.__cls = Button(self.__root, text = "Clear", padx=15, pady=5, 
                    font=("Arial", 10, "bold"), bg = "black", fg = "white", command = self.__clr)
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
        self.clear_table()

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
            # opening the file  
            self.__root.title(os.path.basename(self.__file))
            self.clear_table()
            self.__ax.cla()
            self.__axe.cla()
            self.count = True
            
            w = subprocess.Popen(['TestRecipes','-o',fname],stdout=subprocess.PIPE,stderr=subprocess.PIPE,shell=True)
            output, errors = w.communicate()
            self.__text.insert(END, output)
            
            self.__table.importCSV("temp.csv")
            model = self.__table.model
            model.deleteColumn(0)
            v0 = list(map(int, model.getColumnData(0,filters=[('Recipe Number','','contains','')])))
            model.deleteColumns([0,6,6,6])
            self.__table.redraw()
            
            f = self.freq_count(v0)
            self.y = self.value_gen(f,1)
            self.y1 = list(map(float, model.getColumnData(0,filters=[('B x','','contains','')])))
            self.y2 = list(map(float, model.getColumnData(1,filters=[('B y','','contains','')])))
            self.y3 = list(map(float, model.getColumnData(2,filters=[('B z','','contains','')])))
            v4 = list(map(int, model.getColumnData(3,filters=[('Led Time','','contains','')])))
            v5 = model.getColumnData(4,filters=[('Led Color','','contains','')])
            v6 = list(map(int, model.getColumnData(5,filters=[('Led Intensity','','contains','')])))
            self.y4,self.y5,self.y6 = self.led_val(v5,v6)
            self.x = self.value_gen(v4,1000) # converting milli_s -> seconds
            
            self.graph()
    
    def value_gen(self,a,b):
        val = [0]*len(a)
        for i,j in enumerate(a):
            val[i]=val[i-1]+int(j/b)
        return val
    
    def update_event(self,event):
        a = self.__table.get_row_clicked(event)
        b = self.__table.get_col_clicked(event)
        print(a,b)
    
    def clear_table(self):
        mod = self.__table.model
        r = list(i for i in range(mod.getRowCount()))
        c = list(i for i in range(mod.getColumnCount()))
        self.__table.multiplerowlist = r
        self.__table.multiplecollist = c
        self.__table.clearData()
        self.__table.redraw()
    
    def freq_count(self, vv): 
        freq = {}
        for i in vv: 
            freq[i] = vv.count(i)
        a = list(freq.values())
        return a

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