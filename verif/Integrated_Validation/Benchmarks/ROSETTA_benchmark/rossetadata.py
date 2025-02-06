#!/usr/bin/python

fin = open('Rosseta_bench.txt', 'r')
fout = open('rosseta_data.txt', 'w')

while True:
   line = fin.readline()
   if line.strip()=='$$SOE':
      break

i=0

for line in fin:
   if i % 3 == 1:
      out_str = line
      out_str = out_str.strip()
   elif i % 3 == 2:
      tmp_str = line
      tmp_str = tmp_str.strip()
      out_str += ' ' + tmp_str + '\n'
      fout.write(out_str)
   i += 1 
     
fin.close()
fout.close()

