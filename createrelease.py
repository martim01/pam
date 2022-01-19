import tarfile
import requests

def get_git_releases(user, project, f):
 #get the release details for the project from the GitHub API
 url = 'https://api.github.com/repos/'+user+'/'+project+'/releases'
 r = requests.get(url)
   
 log = '# '+project+'\n'
 log +='## '+r.json()[0]['tag_name']+'\n'
 log += r.json()[0]['body']+'\n\n'

 f.write(log)
 return r.json()[0]['tag_name']

def create_release():
 f = open('release.md', 'w')
 release = get_git_releases('martim01','pam', f)
 get_git_releases('martim01','nmos', f)
 get_git_releases('martim01','ptpmonkey', f)
 get_git_releases('martim01','dnssd', f)
 get_git_releases('martim01','sapserver', f)
 get_git_releases('martim01','restgoose', f)
 get_git_releases('martim01','log', f)  
 f.close()
 return release

def create_tar(filename):
 filename += '.put'
 tarObj = tarfile.open(filename, 'w')
 tarObj.add('release.md')
 tarObj.add('bin/pamupdatemanager', arcname="pamupdatemanager")
 tarObj.add('bin/pam2')
 tarObj.add('bin/dosetup')
 tarObj.add('bin/InitialSetup')
 tarObj.add('lib/')
 tarObj.add('documents/audio_hats.xml')
 tarObj.add('documents/generator/')
 tarObj.add('documents/help/')
 tarObj.add('documents/macaddress.xml')
 tarObj.add('documents/ppmtypes.xml')

 tarObj.close()


if __name__ == '__main__':
 filename = create_release()
 create_tar(filename)
