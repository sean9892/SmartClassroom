# -*- coding: utf-8 -*-

#ComciCrawler.py

connectArduino=False

import time

def installModule(imp,ins=None):
    if ins==None:
        import importlib
        try:
            importlib.import_module(imp)
        except ImportError:
            import pip
            if int(pip.__version__.split('.')[0]) > 9:
                from pip._internal import main
            else:
                from pip import main
            main(['install',imp])
    else:
        import importlib
        try:
            importlib.import_module(imp)
        except ImportError:
            import pip
            if int(pip.__version__.split('.')[0]) > 9:
                from pip._internal import main
            else:
                from pip import main
            main(['install', ins])


installModule('selenium')
installModule('chromedriver-autoinstaller')
installModule('serial','pySerial')

from selenium import webdriver
from selenium.webdriver.support.ui import Select
from selenium.webdriver.support.ui import WebDriverWait
from selenium.webdriver.common.by import By
from selenium.webdriver.support import expected_conditions as EC

import chromedriver_autoinstaller
import serial
import sys
import glob

#범위 내 숫자만 입력
def inpInt_exp_in_rng(inpstr,lo,hi):
    assert(type(inpstr)==str and type(lo)==int and type(hi)==int and lo<=hi)
    ans=lo-1
    while ans<lo or ans>hi:
        ans=int(input(inpstr))
    return ans

def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result

#컴시간 크롤러
class ComciganDGetter:
    #생성자
    def __init__(self,schname,port,headless=True):
        #변수 초기화
        self.schname=schname
        #==================

        options=webdriver.ChromeOptions()
        if headless:
            options.add_argument('headless')
        else:
            pass
        self.driver = webdriver.Chrome(chrome_options=options)#크롬 드라이버 로딩
        self.driver.implicitly_wait(10)

        self.driver.get('http://112.186.146.81:4082/st')#컴시간 접속

        if AVAILPORT:
            self.ser=serial.Serial(port,9600)

    #시리얼 대기
    def waitSerial(self):
        if not connectArduino:
            return
        while self.ser.in_waiting==0:
            pass

    #시리얼 한 줄 읽기
    def ardReadLine(self):
        if connectArduino:
            return self.ser.readline()[:-2].decode()
        else:
            return input(">> ")

    #시간표 접속
    def access(self):
        schname=self.schname
        WebDriverWait(self.driver,10).until(EC.presence_of_all_elements_located)
        while True:
            try:
                self.driver.find_element_by_id('sc').send_keys(schname)
                break
            except:
                pass
        while True:
            try:
                self.driver.find_element_by_xpath('//*[@id="학교찾기"]/table[1]/tbody/tr[2]/td[2]/input[2]').click()
                break
            except:
                pass
        while True:
            try:
                self.driver.find_element_by_link_text(schname).click()
                break
            except:
                pass

    #반 선택
    def selClass(self):
        WebDriverWait(self.driver, 10).until(EC.presence_of_all_elements_located)
        sel=Select(self.driver.find_element_by_name('ba'))
        self.waitSerial()
        selC=int(self.ardReadLine())
        sel.select_by_index(selC)

    #시간표 크롤링
    def getTS(self):
        WebDriverWait(self.driver, 10).until(EC.presence_of_all_elements_located)
        self.schedule=[]
        d=time.localtime().tm_wday
        #토요일 혹은 일요일인 경우(for debug)
        if d>4:
            d=4
        for i in range(8):
            txt=self.driver.find_element_by_xpath('//*[@id="hour"]/table/tbody/tr[%d]/td[%d]'%(i+3,d+2)).text
            if txt=="":
                txt="없음"
            else:
                txt=txt[-2:]
            self.schedule.append(txt)
        for x in self.schedule:
            print(x)
            if connectArduino:
                self.ser.write((str(x) + "\n").encode('utf-8'))

    def end(self):
        try:
            self.ser.close()
        except:
            pass
        self.driver.quit()
        time.sleep(1)

cbsh=None
chromedriver_autoinstaller.install()

if connectArduino:
    ports=serial_ports()
    if len(ports)==0:
        print("Failed: No Arduino")
        exit(-1)
    AVAILPORT=ports[0]
else:
    AVAILPORT=None

try:
    print("Try:")
    cbsh = ComciganDGetter('충북과학고등학교', AVAILPORT, True)
    print("Success: initializing object")
    cbsh.access()
    print("Success: access comci")
    cbsh.selClass()
    print("Success: select class")
    cbsh.getTS()
    print("Success: get time schedule")
    cbsh.end()
    print("Success: exit")
except Exception as e:
    if cbsh is not None and connectArduino:
        cbsh.ser.close()
    print("Failed: ", e)