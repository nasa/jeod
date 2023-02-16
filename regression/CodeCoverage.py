class CodeCoverage:
    def __init__(coverage_format):
        self.coverage_format = coverage_format
        self.command_string = '/gcovr-3.3/gcovr -e "(.*[/]*io_src/|.*[/]*swig/|.*[/]*verif/|.*[/]*test/|.*[/]*object_Linux_.*/)" -r '
        self.find_string =  ' -type f -iname "*.o" -not -path "*/verif/*" -not -path "*/io_src/*" -not -path "*/swig/*" -not -path "*/test/*" 2>/dev/null | sort | uniq'

        self.xml_string =  " --xml-pretty -o coverage.xml"
        self.html_string = " --html --html-details --html-absolute-paths  -o coverage.html"


    def execute(model_dir):
        test_path = model_dir + ("/code_coverage")

        shutil.rmtree(test_path, True)
        os.mkdir(test_path)

        tprint("   build coverage file list ...")
        gc_dic = buildGcDic(model_dir)

        tprint("   copy coverage files ...")
        cp_gc_files(gc_dic)

        with changeDirectory(test_path):
            gcovr_cmd = scriptDir + command_string + model_dir

            #generage xml output
            if 1==self.coverage_format%2:
              tprint("   generate xml report ...")
              cov_cmd = gcovr_cmd + " --xml-pretty -o coverage.xml"
              sys_cmd(cov_cmd)

            #generage html output
            if 2<= self.coverage_format%4:
              tprint("   generate html report ...")
              cov_cmd = gcovr_cmd + " --html --html-details --html-absolute-paths  -o coverage.html"
              sys_cmd(cov_cmd)

            tprint("   clean up ...")
            clean_gc_files(gc_dic)

            if 1==self.coverage_format%2:
              tprint("   count the not-used source files ...")
              count_not_built_in_files(model_dir)


    #build the dictionary of .gcxx file location and its source code location
    def buildGcDic(model_dir):
      gc_dic = {}
      find_cmd = 'find ' + model_dir + self.find_string
      find_res = sys_cmd(find_cmd, False)
      obj_list = find_res.splitlines()
      for obj in obj_list:
        gcno_file = obj[:-2] + ".gcno"
        if (not os.path.isfile(gcno_file)) or (not os.path.exists(gcno_file)):
          continue

        src_loc = get_src_loc(obj, model_dir)
        if src_loc is None:
          continue
        if (not os.path.isdir(src_loc)) or (not os.path.exists(src_loc)):
          tprint("WARNING: The source code directory " + src_loc + " dosen't exist!")
          continue
        gc_dic[obj[:-2] + ".gc*"] = src_loc
      return gc_dic


    #parse the source code file from the given object file
    def get_src_loc(obj_file, model_dir):
      dump_cmd = 'objdump -W ' + obj_file + ' 2>/dev/null | grep "^.*DW_AT_name.*$" | grep "\\.\\(cpp\\|cc\\|c\\|\\f\\)" | grep -v "_GLOBAL__I_" | awk \'{print $NF;}\''
      dump_res = sys_cmd(dump_cmd, False)
      src_path_list = dump_res.splitlines()
      if len(src_path_list) == 0:
        return None

      src_path = os.path.split(src_path_list[0].strip())[0].strip()
      if len(src_path)==0 or src_path[0]!='/':
        dump_cmd = 'objdump -W ' + obj_file + ' 2>/dev/null | grep "^.*DW_AT_comp_dir.*[ \\t][ \\t]*' + model_dir + '"' + " | awk '{print $NF;}'"
        dump_res = sys_cmd(dump_cmd, False)
        comp_path_list = dump_res.splitlines()
        if len(comp_path_list) == 0:
          return None
        comp_path = comp_path_list[0].strip()
        if len(comp_path)==0 or comp_path[0]!='/':
          return None
        src_path = os.path.abspath(os.path.join(comp_path, src_path))

      obj_path = os.path.split(obj_file)[0].strip()
      if obj_path == src_path:
        return None
      if src_path.find(model_dir) == -1:
        return None
      if re.search("(/verif/|/io_src/|/swig/|/test/)", src_path) !=  None:
        return None
      return src_path


    #Copy the coverage files into source code location
    def cp_gc_files(gc_dic):
      for src, tgt  in gc_dic.items():
        for file in glob.glob(src):
          shutil.copy(file, tgt)
      return 0

    #clean the coverage files from source code locaton
    def clean_gc_files(gc_dic):
      code_path_set = set([])
      for src, tgt in gc_dic.items():
        tgt_file = tgt + "/" + os.path.split(src)[1].strip()
        for file in glob.glob(tgt_file):
          os.remove(file)

    #count the no-built-in files in the given model_dir
    def count_not_built_in_files(model_dir):
      built_in_set = set([])
      cmd_txt = 'grep -o \'filename=".*"\' ' + model_dir + '/code_coverage/coverage.xml | awk -F\'"\' \'{print $2;}\' | sort | uniq'
      cmd_res = sys_cmd(cmd_txt)
      built_in_files = cmd_res.splitlines()
      for item in built_in_files:
        file_path = os.path.abspath(os.path.join(model_dir, item))
        built_in_set.add(file_path)

      cmd_txt = 'find ' + model_dir + ' -type f -iname "*.c" -o -iname "*.cc" -o -iname "*.cpp" -o -iname "*.f90" | grep -v "\\(/io_src/\\|/swig/\\|/verif/\\|/test/\\)" | sort | uniq'
      cmd_res = sys_cmd(cmd_txt)
      model_files = cmd_res.splitlines()

      log_file = open(os.path.abspath(os.path.join(model_dir, 'code_coverage/not_used_files.txt')), 'w')
      cnt = 0
      for item in model_files:
        if item not in built_in_set:
          log_file.write(item)
          log_file.write("\n")
          cnt += 1
      log_file.close()

      cnt_file = open(os.path.abspath(os.path.join(model_dir, 'code_coverage/count_file.txt')), 'w')
      cnt_file.write('Total Source Files:'+str(len(model_files))+'\n')
      cnt_file.write('Not_Used_Source_files:'+str(cnt)+'\n')
      cnt_file.close()


