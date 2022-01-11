import subprocess

def get_git_log(project, commit):
   cmd = 'git'
   log = 'log'
   grep="--grep="
   log_format ='--pretty=%s'

   
   
   output = subprocess.check_output([cmd, log, grep+project+commit, log_format]).decode('utf-8')
   lines = output.splitlines()

   print(lines)

def get_git_project_log(project):
   improvement = "improvement"
   feat = "feat"
   perf = "perf"
   fix = "fix"
      
   get_git_log(project, feat)
   get_git_log(project, improvement)
   get_git_log(project, perf)
   get_git_log(project, fix)

def create_release():
    #find all CMakelists.txt files
    #get the git log for the project name defined in the file
    #add the binary path/name to what our tar list
    ##

    get_git_project_log('')








if __name__ == '__main__':
    create_release()