import os
import xml.etree.ElementTree as ET 

varPanel = True
varLevel = False
varFFT = False
varMonitor = True

dict = { "@TYPE@" : "monitor",
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
options = []


def createDirectory(dir):
    try:
        os.mkdir(dir)
        print("Directory ", dir, " created.")
    except FileExistsError:
        print("Directory ", dir, " already exits.")
    
def createFile(finName, foutName):
    #input file
    fin = open(finName, "rt")
    #output file
    fout = open("../plugins/"+dict["@PLUGIN@"]+"/"+foutName, "wt")

    for line in fin:
        #remove lines that are only in the monitor plugin
        varAdd = True
        if line.find("$MONITOR$") != -1:
            if varMonitor == True:
                print("replace: "+line)
                line = line.replace("$MONITOR$", "")
            else:
                varAdd = False
        if varAdd == True:
            for key in dict:
                line = line.replace(key, dict[key])
        
            fout.write(line)


def createOptionPanel(name, finExt, dest):
    outDest = "../plugins/"+dict["@PLUGIN@"]+"/pnl"+name+"."+finExt;
    if dest != "":
        outDest = "../plugins/"+dict["@PLUGIN@"]+"/"+dest+"/pnl"+name+"."+finExt;
    
    fin = open("options/pnloptions."+finExt+".in")
    fout = open(outDest, "wt")

    for line in fin:
        for key in dict:
            line = line.replace(key, dict[key])
        line = line.replace("@OPTIONS@", name)
        fout.write(line)


def createOptionPanels():
    for name in options:
        createOptionPanel(name, "h", "")
        createOptionPanel(name, "cpp", "")
        createOptionPanel(name, "wxs", "wxsmith")


def AddReleaseLibs(linkerNode, varFFT, varLevel):
    if varFFT == True:
            linkerNode.append(ET.Element("Add", {"library" : "pamfft"}))
    if varLevel == True:
        linkerNode.append(ET.Element("Add", {"library" : "pamlevel"}))
        
def AddDebugLibs(linkerNode, varFFT, varLevel):
    if varFFT == True:
        linkerNode.append(ET.Element("Add", {"library" : "pamfftd"}))
    if varLevel == True:
        linkerNode.append(ET.Element("Add", {"library" : "pamleveld"}))


def createCBProject(varPanel, varFFT, varLevel):
    tree = ET.parse("project.xml.in")
    root = tree.getroot()
    projectNode = root.find("Project")
    projectNode.append(ET.Element("Option", {"title" : dict["@PLUGIN@"]}))

    buildNode = projectNode.find("Build")
    for childNode in buildNode:
        linkerNode = childNode.find("Linker")
        
        if childNode.attrib["title"] == "Windows Release":
            childNode.append(ET.Element("Option", {"output": "../../lib/"+dict["@TYPE@"]+"/"+dict["@PLUGIN@"]+".dll", "prefix_auto" : "0" , "extension_auto" : "0"}))
            AddReleaseLibs(linkerNode, varFFT, varLevel)

        if childNode.attrib["title"] == "Linux Release":
            childNode.append(ET.Element("Option", {"output": "../../lib/"+dict["@TYPE@"]+"/"+dict["@PLUGIN@"]+".so", 
                                                   "imp_lib" : "$(TARGET_OUTPUT_DIR)$(TARGET_OUTPUT_BASENAME).a",
                                                   "def_file" : "$(TARGET_OUTPUT_DIR)$(TARGET_OUTPUT_BASENAME).def",
                                                   "prefix_auto" : "0",
                                                   "extension_auto" : "0"}))
            AddReleaseLibs(linkerNode, varFFT, varLevel)

        if childNode.attrib["title"] == "Windows Debug":
            childNode.append(ET.Element("Option", {"output": "../../lib/"+dict["@TYPE@"]+"/debug/"+dict["@PLUGIN@"]+".dll", "prefix_auto" : "0" , "extension_auto" : "0"}))
            AddDebugLibs(linkerNode, varFFT, varLevel)
        if childNode.attrib["title"] == "Linux Debug":
            childNode.append(ET.Element("Option", {"output": "../../lib/"+dict["@TYPE@"]+"/debug/"+dict["@PLUGIN@"]+".so", 
                                                   "imp_lib" : "$(TARGET_OUTPUT_DIR)$(TARGET_OUTPUT_BASENAME).a",
                                                   "def_file" : "$(TARGET_OUTPUT_DIR)$(TARGET_OUTPUT_BASENAME).def",
                                                   "prefix_auto" : "0",
                                                   "extension_auto" : "0"}))
            AddDebugLibs(linkerNode, varFFT, varLevel)


        
    compileNode = projectNode.find("Compiler")
    if varFFT == True:
        compileNode.append(ET.Element("Add", {"directory" : "../../pamfft"}))
        compileNode.append(ET.Element("Add", {"directory" : "$(#kiss.include)"}))
        compileNode.append(ET.Element("Add", {"directory" : "$(#kiss.include)/tools"}))
    if varLevel == True:
        compileNode.append(ET.Element("Add", {"directory" : "../../pamlevel"}))

    projectNode.append(ET.Element("Unit", {"filename" : dict["@PLUGIN@"]+"builder.h"}))
    projectNode.append(ET.Element("Unit", {"filename" : dict["@PLUGIN@"]+"builder.cpp"}))
    projectNode.append(ET.Element("Unit", {"filename" : dict["@PLUGIN@"]+"register.h"}))
    projectNode.append(ET.Element("Unit", {"filename" : dict["@PLUGIN@"]+"register.cpp"}))

    if varPanel == True:
        projectNode.append(ET.Element("Unit", {"filename" : dict["@PLUGIN@"]+"panel.h"}))
        projectNode.append(ET.Element("Unit", {"filename" : dict["@PLUGIN@"]+"panel.cpp"}))
        projectNode.append(ET.Element("Unit", {"filename" : "wxsmith/"+dict["@PLUGIN@"]+"panel.wxs"}))

    else:
        projectNode.append(ET.Element("Unit", {"filename" : dict["@PLUGIN@"]+"meter.h"}))
        projectNode.append(ET.Element("Unit", {"filename" : dict["@PLUGIN@"]+"meter.cpp"}))
        
    for panel in options:
        projectNode.append(ET.Element("Unit", {"filename" : "pnl"+panel+".cpp"}))
        projectNode.append(ET.Element("Unit", {"filename" : "pnl"+panel+".h"}))
        projectNode.append(ET.Element("Unit", {"filename" : "wxsmith/pnl"+panel+".wxs"}))

    if len(options) > 0 or varPanel == True:
        extensionNode = projectNode.find("Extensions")
        if extensionNode == None:
            print("Could not find extension node...")
        else:
            wxsNode = ET.Element("wxsmith", {"version" : "1"})
            resourcesNode = ET.Element("resources")
            if varPanel == True:
                resourcesNode.append(ET.Element("wxPanel", {"wxs" : "wxsmith/"+dict["@PLUGIN@"]+"panel.wxs", 
                                                            "src" : dict["@PLUGIN@"]+"panel.cpp", 
                                                            "hdr" : dict["@PLUGIN@"]+"panel.h",
                                                            "name": dict["@PLUGIN@"]+"Panel",
                                                            "i18n" : "1",
                                                            "fwddecl" : "0",
                                                            "language" : "cpp"}))
            for panel in options:
                resourcesNode.append(ET.Element("wxPanel", {"wxs" : "wxsmith/pnl"+panel+".wxs", 
                                                            "src" : "pnl"+panel+".cpp", 
                                                            "hdr" : "pnl"+panel+".h",
                                                            "name": "pnl"+panel,
                                                            "i18n" : "1",
                                                            "fwddecl" : "0",
                                                            "language" : "cpp"}))
            wxsNode.append(resourcesNode)
            extensionNode.append(wxsNode)

    tree.write("../plugins/"+dict["@PLUGIN@"]+"/"+dict["@PLUGIN@"]+".cbp")




def yesNo(question, default="no"):
    if default is None:
        prompt = " [y/n]"
    elif default == 'yes':
        prompt = " [Y/n]"
    elif default == 'no':
        prompt = " [y/N]"
    else:
        raise ValueError(f"Unknown setting '{default}' for default.")

    while True:
        try:
            resp = input(question+prompt).strip().lower()
            if default is not None and resp == '':
                return default == 'yes'
            else:
                if resp == 'y' or resp == 'yes':
                    return True
                else:
                    return False
        except ValueError:
            print("Please respong with 'yes' or 'no (or 'y' or 'n')/.\n")


def createVariables():
    global dict, varMonitor, varPanel, varFFT, varLevel, options

    dict["@PLUGIN@"] = input("Enter plugin name: ")
    varMonitor = yesNo("Is this a monitor plugin? ", "yes")
    varPanel = yesNo("Use a base panel? ", "yes")
    varFFT = yesNo("Use FFT library? ")
    varLevel = yesNo("Use Level library? ")
    dict["@DESCRIPTION@"] = input("Enter a description of the plugin: ")

    if varMonitor == True:
        varOptions = "_"
        while varOptions != "":
            varOptions = input("Enter name of options panel to add without spaces or return to stop: ")
            if varOptions != "":
                options.append(varOptions)
    else:
        dict["@TYPE@"] = "test"
        dict["@TYPEU@"] = "Test"


    if varPanel == True:
        dict["@DISPLAY_FILE@"] = 'panel'
        dict["@DISPLAY_TYPE@"] = 'Panel'
        dict["@SOURCES@"] = "\""+dict["@PLUGIN@"]+"panel.cpp\" \""+dict["@PLUGIN@"]+"builder.cpp\" \""+dict["@PLUGIN@"]+"register.cpp\""
    else:
        dict["@DISPLAY_FILE@"] = 'meter'
        dict["@DISPLAY_TYPE@"] = 'Meter'
        dict["@SOURCES@"] =  "\""+dict["@PLUGIN@"]+"meter.cpp\" \""+dict["@PLUGIN@"]+"builder.cpp\" \""+dict["@PLUGIN@"]+"register.cpp\""
        

    if varFFT == True:
        dict["@FFT_INCLUDE@"]  = "include_directories(\"../../pamfft/\" \"../../external/kiss_fft130\" \"../../external/kiss_fft130/tools\")"
        dict["@FFT_LIB@"]  = "target_link_libraries("+dict["@PLUGIN@"]+" optimized pamfft debug pamfftd)"

    if varLevel == True:
        dict["@LEVEL_INCLUDE@"]  = "include_directories(\"../../pamlevel\")"
        dict["@LEVEL_LIB@"]  = "target_link_libraries("+dict["@PLUGIN@"]+" optimized pamlevel debug pamleveld)"

    for name in options:
        dict["@SOURCES@"] += " \"pnl"+name+".cpp\""
        if dict["@CREATE_OPTIONS@"] != "":
            dict["@CREATE_OPTIONS@"] += ","
            dict["@INCLUDE_OPTIONS@"] += "\n"

        dict["@CREATE_OPTIONS@"] += " {\""+name+"\", new pnl"+name+"(pParent, this)}"
        dict["@INCLUDE_OPTIONS@"] += "#include \"pnl"+name+".h\""

def EditMainCMakeFile():
    fin = open("../CMakeLists.txt", "rt")
    fout = open("../CMakeLists.tmp", "wt")
    for line in fin:
        if line == "message(STATUS \"Configure pam2\")\n":
            fout.write("message(STATUS \"Configure "+dict["@PLUGIN@"]+"\")\n")
            fout.write("add_subdirectory(plugins/"+dict["@PLUGIN@"]+")\n")
        if line != "add_subdirectory(plugins/"+dict["@PLUGIN@"]+")\n" and line != "message(STATUS \"Configure "+dict["@PLUGIN@"]+"\")\n":
            fout.write(line)
    fin.close()
    fout.close()
    os.remove("../CMakeLists.txt")
    os.rename("../CMakeLists.tmp", "../CMakeLists.txt")

#MAIN CODE
createVariables()
print("Creating plugin...")
print("Create directories...")
createDirectory("../plugins/"+dict["@PLUGIN@"])
createDirectory("../plugins/"+dict["@PLUGIN@"]+"/wxsmith")

print("Create source files...")
createFile("builder.cpp.in", dict["@PLUGIN@"]+"builder.cpp")
createFile("builder.h.in", dict["@PLUGIN@"]+"builder.h")
createFile("register.cpp.in", dict["@PLUGIN@"]+"register.cpp")
createFile("register.h.in", dict["@PLUGIN@"]+"register.h")


if varPanel == True:
    createDirectory("../plugins/"+dict["@PLUGIN@"]+"/wxsmith")
    createFile("./panel/panel.cpp.in", dict["@PLUGIN@"]+"panel.cpp")
    createFile("./panel/panel.h.in", dict["@PLUGIN@"]+"panel.h")
    createFile("./panel/wxsmith/panel.wxs.in", "wxsmith/"+dict["@PLUGIN@"]+"panel.wxs")

else:
    createFile("./meter/meter.cpp.in", dict["@PLUGIN@"]+"meter.cpp")
    createFile("./meter/meter.h.in", dict["@PLUGIN@"]+"meter.h")
    

print("Create option panels source files...")
createOptionPanels()

print("Create project files...")
createFile("CMakeLists.txt.in", "CMakeLists.txt")
createCBProject(varPanel, varFFT, varLevel)
EditMainCMakeFile()
print("Finished!")
