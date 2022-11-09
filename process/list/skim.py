file1 = open('full_list.txt', 'r')
Lines1 = file1.readlines()
for line in Lines1:
    line = line.strip('\n')
    line = line.strip('\t')
    x=line.split("/")
    print("%s"%(x[-1]))
