import os
from urllib.request import build_opener
import xml.etree.ElementTree as ET 


def yesNo(question, default="no") -> bool:
    if default is None:
        prompt = " [y/n]"
    elif default == 'yes':
        prompt = " [Y/n]"
    elif default == 'no':
        prompt = " [y/N]"
    else:
        raise ValueError(f"Unknown setting '{default}' for default.")

    while True:
        resp = input(question+prompt).strip().lower()
        if default is not None and resp == '':
            return default == 'yes'
        else:
            if resp == 'y' or resp == 'yes':
                return True
            elif resp == 'n' or resp == 'no':
                return False
            else:
                print("Please respond with 'yes' or 'no (or 'y' or 'n')!")

def createDirectory(dir):
        try:
            os.mkdir(dir)
            print("Directory ", dir, " created.")
        except FileExistsError:
            print("Directory ", dir, " already exits.")
    

class PluginBuilder:
    def __init__(self) -> None:
        self.varPanel = True
        self.varLevel = False
        self.varFFT = False
        self.varMonitor = True

        self.dict = { "@TYPE@" : "monitor",
                    "@TYPEU@": "Monitor",
                    "@PLUGIN@" : "",  
                    "@DISPLAY_FILE@" : "",
                    "@DISPLAY_TYPE@" : "",
                    "@DESCRIPTION@" : "",
                    "@FFT_INCLUDE@" : "",
                    "@FFT_INCLUDE_CBP@" : "",
                    "@FFT_LIB@" : "",
                    "@FFT_LIB_CBP@" : "",
                    "@FFT_LIB_CBPD@" : "",
                    "@LEVEL_INCLUDE@" : "",
                    "@LEVEL_INCLUDE_CBP@" : "",
                    "@LEVEL_LIB@" : "",
                    "@LEVEL_LIB_CBP@" : "",
                    "@LEVEL_LIB_CBPD@" : "",
                    "@SOURCES@" : "",
                    "@PANEL_WXS@" : "",
                    "@OPTION_UNITS@" : "",
                    "@CREATE_OPTIONS@" : "",
                    "@INCLUDE_OPTIONS@" : ""
                    }
        self.options = []


    
    def createFile(self, finName, foutName):
        #input file
        fin = open(finName, "rt")
        #output file
        fout = open("../plugins/"+self.dict["@PLUGIN@"]+"/"+foutName, "wt")

        for line in fin:
            #remove lines that are only in the monitor plugin
            varAdd = True
            if line.find("$MONITOR$") != -1:
                if self.varMonitor == True:
                    line = line.replace("$MONITOR$", "")
                else:
                    varAdd = False
            if varAdd == True:
                for key in self.dict:
                    line = line.replace(key, self.dict[key])
            
                fout.write(line)


    def createOptionPanel(self, name, finExt, dest):
        outDest = "../plugins/"+self.dict["@PLUGIN@"]+"/pnl"+name+"."+finExt;
        if dest != "":
            outDest = "../plugins/"+self.dict["@PLUGIN@"]+"/"+dest+"/pnl"+name+"."+finExt;
        
        fin = open("files/options/pnloptions."+finExt+".in")
        fout = open(outDest, "wt")

        for line in fin:
            for key in self.dict:
                line = line.replace(key, self.dict[key])
            line = line.replace("@OPTIONS@", name)
            fout.write(line)


    def createOptionPanels(self):
        for name in self.options:
            self.createOptionPanel(name, "h", "")
            self.createOptionPanel(name, "cpp", "")
            self.createOptionPanel(name, "wxs", "wxsmith")


    def AddReleaseLibs(self, linkerNode):
        if self.varFFT == True:
                linkerNode.append(ET.Element("Add", {"library" : "pamfft"}))
        if self.varLevel == True:
            linkerNode.append(ET.Element("Add", {"library" : "pamlevel"}))
        
    def AddDebugLibs(self, linkerNode):
        if self.varFFT == True:
            linkerNode.append(ET.Element("Add", {"library" : "pamfftd"}))
        if self.varLevel == True:
            linkerNode.append(ET.Element("Add", {"library" : "pamleveld"}))


    def createCBProject(self):
        tree = ET.parse("files/project.xml.in")
        root = tree.getroot()
        projectNode = root.find("Project")
        projectNode.append(ET.Element("Option", {"title" : self.dict["@PLUGIN@"]}))

        buildNode = projectNode.find("Build")
        for childNode in buildNode:
            linkerNode = childNode.find("Linker")
            
            if childNode.attrib["title"] == "Windows Release":
                childNode.append(ET.Element("Option", {"output": "../../lib/"+self.dict["@TYPE@"]+"/"+self.dict["@PLUGIN@"]+".dll", "prefix_auto" : "0" , "extension_auto" : "0"}))
                self.AddReleaseLibs(linkerNode)

            if childNode.attrib["title"] == "Linux Release":
                childNode.append(ET.Element("Option", {"output": "../../lib/"+self.dict["@TYPE@"]+"/"+self.dict["@PLUGIN@"]+".so", 
                                                    "imp_lib" : "$(TARGET_OUTPUT_DIR)$(TARGET_OUTPUT_BASENAME).a",
                                                    "def_file" : "$(TARGET_OUTPUT_DIR)$(TARGET_OUTPUT_BASENAME).def",
                                                    "prefix_auto" : "0",
                                                    "extension_auto" : "0"}))
                self.AddReleaseLibs(linkerNode)

            if childNode.attrib["title"] == "Windows Debug":
                childNode.append(ET.Element("Option", {"output": "../../lib/"+self.dict["@TYPE@"]+"/debug/"+self.dict["@PLUGIN@"]+".dll", "prefix_auto" : "0" , "extension_auto" : "0"}))
                self.AddDebugLibs(linkerNode)
            if childNode.attrib["title"] == "Linux Debug":
                childNode.append(ET.Element("Option", {"output": "../../lib/"+self.dict["@TYPE@"]+"/debug/"+self.dict["@PLUGIN@"]+".so", 
                                                    "imp_lib" : "$(TARGET_OUTPUT_DIR)$(TARGET_OUTPUT_BASENAME).a",
                                                    "def_file" : "$(TARGET_OUTPUT_DIR)$(TARGET_OUTPUT_BASENAME).def",
                                                    "prefix_auto" : "0",
                                                    "extension_auto" : "0"}))
                self.AddDebugLibs(linkerNode)


            
        compileNode = projectNode.find("Compiler")
        if self.varFFT == True:
            compileNode.append(ET.Element("Add", {"directory" : "../../pamfft"}))
            compileNode.append(ET.Element("Add", {"directory" : "$(#kiss.include)"}))
            compileNode.append(ET.Element("Add", {"directory" : "$(#kiss.include)/tools"}))
        if self.varLevel == True:
            compileNode.append(ET.Element("Add", {"directory" : "../../pamlevel"}))

        projectNode.append(ET.Element("Unit", {"filename" : self.dict["@PLUGIN@"]+"builder.h"}))
        projectNode.append(ET.Element("Unit", {"filename" : self.dict["@PLUGIN@"]+"builder.cpp"}))
        projectNode.append(ET.Element("Unit", {"filename" : self.dict["@PLUGIN@"]+"register.h"}))
        projectNode.append(ET.Element("Unit", {"filename" : self.dict["@PLUGIN@"]+"register.cpp"}))

        if self.varPanel == True:
            projectNode.append(ET.Element("Unit", {"filename" : self.dict["@PLUGIN@"]+"panel.h"}))
            projectNode.append(ET.Element("Unit", {"filename" : self.dict["@PLUGIN@"]+"panel.cpp"}))
            projectNode.append(ET.Element("Unit", {"filename" : "wxsmith/"+self.dict["@PLUGIN@"]+"panel.wxs"}))

        else:
            projectNode.append(ET.Element("Unit", {"filename" : self.dict["@PLUGIN@"]+"meter.h"}))
            projectNode.append(ET.Element("Unit", {"filename" : self.dict["@PLUGIN@"]+"meter.cpp"}))
            
        for panel in self.options:
            projectNode.append(ET.Element("Unit", {"filename" : "pnl"+panel+".cpp"}))
            projectNode.append(ET.Element("Unit", {"filename" : "pnl"+panel+".h"}))
            projectNode.append(ET.Element("Unit", {"filename" : "wxsmith/pnl"+panel+".wxs"}))

        if len(self.options) > 0 or self.varPanel == True:
            extensionNode = projectNode.find("Extensions")
            if extensionNode == None:
                print("Could not find extension node...")
            else:
                wxsNode = ET.Element("wxsmith", {"version" : "1"})
                resourcesNode = ET.Element("resources")
                if self.varPanel == True:
                    resourcesNode.append(ET.Element("wxPanel", {"wxs" : "wxsmith/"+self.dict["@PLUGIN@"]+"panel.wxs", 
                                                                "src" : self.dict["@PLUGIN@"]+"panel.cpp", 
                                                                "hdr" : self.dict["@PLUGIN@"]+"panel.h",
                                                                "name": self.dict["@PLUGIN@"]+"Panel",
                                                                "i18n" : "1",
                                                                "fwddecl" : "0",
                                                                "language" : "cpp"}))
                for panel in self.options:
                    resourcesNode.append(ET.Element("wxPanel", {"wxs" : "wxsmith/pnl"+panel+".wxs", 
                                                                "src" : "pnl"+panel+".cpp", 
                                                                "hdr" : "pnl"+panel+".h",
                                                                "name": "pnl"+panel,
                                                                "i18n" : "1",
                                                                "fwddecl" : "0",
                                                                "language" : "cpp"}))
                wxsNode.append(resourcesNode)
                extensionNode.append(wxsNode)

        tree.write("../plugins/"+self.dict["@PLUGIN@"]+"/"+self.dict["@PLUGIN@"]+".cbp")


    def createVariables(self):

        while self.dict["@PLUGIN@"] == "":
            self.dict["@PLUGIN@"] = input("Enter plugin name: ").strip()


        self.varMonitor = yesNo("Is this a monitor plugin? ", "yes")
        self.varPanel = yesNo("Use a base panel? ", "yes")
        self.varFFT = yesNo("Use FFT library? ")
        self.varLevel = yesNo("Use Level library? ")
        self.dict["@DESCRIPTION@"] = input("Enter a description of the plugin: ")

        if self.varMonitor == True:
            varOptions = "_"
            while varOptions != "":
                varOptions = input("Enter name of options panel to add without spaces or return to stop: ")
                if varOptions != "":
                    self.options.append(varOptions)
        else:
            self.dict["@TYPE@"] = "test"
            self.dict["@TYPEU@"] = "Test"


        if self.varPanel == True:
            self.dict["@DISPLAY_FILE@"] = 'panel'
            self.dict["@DISPLAY_TYPE@"] = 'Panel'
            self.dict["@SOURCES@"] = "\""+self.dict["@PLUGIN@"]+"panel.cpp\" \""+self.dict["@PLUGIN@"]+"builder.cpp\" \""+self.dict["@PLUGIN@"]+"register.cpp\""
        else:
            self.dict["@DISPLAY_FILE@"] = 'meter'
            self.dict["@DISPLAY_TYPE@"] = 'Meter'
            self.dict["@SOURCES@"] =  "\""+self.dict["@PLUGIN@"]+"meter.cpp\" \""+self.dict["@PLUGIN@"]+"builder.cpp\" \""+self.dict["@PLUGIN@"]+"register.cpp\""
            

        if self.varFFT == True:
            self.dict["@FFT_INCLUDE@"]  = "include_directories(\"../../pamfft/\" \"../../external/kiss_fft130\" \"../../external/kiss_fft130/tools\")"
            self.dict["@FFT_LIB@"]  = "target_link_libraries("+self.dict["@PLUGIN@"]+" optimized pamfft debug pamfftd)"

        if self.varLevel == True:
            self.dict["@LEVEL_INCLUDE@"]  = "include_directories(\"../../pamlevel\")"
            self.dict["@LEVEL_LIB@"]  = "target_link_libraries("+self.dict["@PLUGIN@"]+" optimized pamlevel debug pamleveld)"

        for name in self.options:
            self.dict["@SOURCES@"] += " \"pnl"+name+".cpp\""
            if self.dict["@CREATE_OPTIONS@"] != "":
                self.dict["@CREATE_OPTIONS@"] += ","
                self.dict["@INCLUDE_OPTIONS@"] += "\n"

            self.dict["@CREATE_OPTIONS@"] += " {\""+name+"\", new pnl"+name+"(pParent, this)}"
            self.dict["@INCLUDE_OPTIONS@"] += "#include \"pnl"+name+".h\""

    def EditMainCMakeFile(self):
        fin = open("../CMakeLists.txt", "rt")
        fout = open("../CMakeLists.tmp", "wt")
        varAdded = False
        for line in fin:
            if line == "#Add new plugin projects above this line\n":
                fout.write("message(STATUS \"Configure "+self.dict["@PLUGIN@"]+"\")\n")
                fout.write("add_subdirectory(plugins/"+self.dict["@PLUGIN@"]+")\n")
                varAdded = True
            if line != "add_subdirectory(plugins/"+self.dict["@PLUGIN@"]+")\n" and line != "message(STATUS \"Configure "+self.dict["@PLUGIN@"]+"\")\n":
                fout.write(line)

        if varAdded == False:
            fout.write("message(STATUS \"Configure "+self.dict["@PLUGIN@"]+"\")\n")
            fout.write("add_subdirectory(plugins/"+self.dict["@PLUGIN@"]+")\n")
            fout.write("#Add new plugin projects above this line\n")

        fin.close()
        fout.close()
        os.remove("../CMakeLists.txt")
        os.rename("../CMakeLists.tmp", "../CMakeLists.txt")

    def Run(self):
        #MAIN CODE
        self.createVariables()
        print("Creating plugin...")
        print("Create directories...")
        createDirectory("../plugins/"+self.dict["@PLUGIN@"])
        createDirectory("../plugins/"+self.dict["@PLUGIN@"]+"/wxsmith")

        print("Create source files...")
        self.createFile("files/builder.cpp.in", self.dict["@PLUGIN@"]+"builder.cpp")
        self.createFile("files/builder.h.in", self.dict["@PLUGIN@"]+"builder.h")
        self.createFile("files/register.cpp.in", self.dict["@PLUGIN@"]+"register.cpp")
        self.createFile("files/register.h.in", self.dict["@PLUGIN@"]+"register.h")


        if self.varPanel == True:
            createDirectory("../plugins/"+self.dict["@PLUGIN@"]+"/wxsmith")
            self.createFile("files/panel/panel.cpp.in", self.dict["@PLUGIN@"]+"panel.cpp")
            self.createFile("files/panel/panel.h.in", self.dict["@PLUGIN@"]+"panel.h")
            self.createFile("files/panel/wxsmith/panel.wxs.in", "wxsmith/"+self.dict["@PLUGIN@"]+"panel.wxs")
        else:
            self.createFile("files/meter/meter.cpp.in", self.dict["@PLUGIN@"]+"meter.cpp")
            self.createFile("files/meter/meter.h.in", self.dict["@PLUGIN@"]+"meter.h")
            

        print("Create option panels source files...")
        self.createOptionPanels()

        print("Create project files...")
        self.createFile("files/CMakeLists.txt.in", "CMakeLists.txt")
        self.createCBProject()
        self.EditMainCMakeFile()
        print("Finished!")


os.chdir(os.path.dirname(os.path.abspath(__file__)))
builder = PluginBuilder()
builder.Run()