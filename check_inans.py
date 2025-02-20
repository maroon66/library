#!/bin/python3

import sys
import os
import shutil
import re
import glob
import tempfile
import subprocess
import filecmp

verdicts=['RE','WA','AC']

def testexe(exe):
	priority=2
	tests=[]
	for f in sorted(os.listdir('.')):
		with tempfile.NamedTemporaryFile() as g:
			root, ext = os.path.splitext(f)
			if ext == '.in':
				infile=root+'.in'
				outfile=root+'.ans'
				print('test',exe,infile,file=sys.stderr)
				userres=subprocess.run(exe,stdin=open(infile),stdout=g.file)
				if userres.returncode!=0:
					if priority>0:
						priority=0
						tests.clear()
					if priority==0:
						tests.append(f)
				else:
					if filecmp.cmp(outfile,g.name):
						if priority>2:
							priority=2
							tests.clear()
						if priority==2:
							tests.append(f)
					else:
						if priority>1:
							priority=1
							tests.clear()
						if priority==1:
							tests.append(f)
	
	if priority==2:
		print(exe,verdicts[priority])
	else:
		print(exe,verdicts[priority],tests)
	sys.stdout.flush()

testexe('./main')


