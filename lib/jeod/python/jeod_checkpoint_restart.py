"""
This python module defines the functions create_checkpoint and
restore_from_checkpoint in the module jeod_checkpoint_restart.
The former drops a checkpoint at the specified checkpoint time
while the latter restores from checkpoint.

NOTE: This is a specific module tailored to JEOD checkpoint/restart
testing and may not be work outside of this context.
"""

import trick
import textwrap
import string


def create_checkpoint (checkpoint_time, restart_end_time) :
  """
  Drop a checkpoint at the specified checkpoint_time.
  On restart, the simulation end time will be the specified
  restart_end_time rather than the current end time.
  """

  trick.TMM_hexfloat_checkpoint(True)

  action = textwrap.dedent(string.Template("""
    trick.sim_services.exec_set_terminate_time($restart_end)
    trick.checkpoint()
    trick.sim_services.exec_set_terminate_time($this_end)
  """).substitute({
    'restart_end' : restart_end_time,
    'this_end' : trick.sim_services.exec_get_terminate_time() }))

  checkpoint_event = trick.new_event("checkpoint")
  checkpoint_event.condition(0,
    "trick.exec_get_sim_time() == " + str(checkpoint_time))
  checkpoint_event.condition_all()
  checkpoint_event.set_cycle(checkpoint_time)
  checkpoint_event.action(0, action)
  checkpoint_event.activate()

  trick.add_event(checkpoint_event)

  return


def restore_from_checkpoint (checkpoint_dir, checkpoint_file) :
  """
  Restore from the checkpoint files indicated by the arguments.

  checkpoint_dir: Directory relative to the executable directory
  that contains the checkpoint file.
  checkpoint file: Name of the Trick checkpoint file within
  the checkpoint_dir.

  The JEOD checkpoint file is assumed to be the Trick checkpoint
  file prefixed by 'jeod_'.
  """

  """
  To get the logged data to align and write to the correct restart directory,
  copy the checkpoint file and update the RUN directory names from the checkpoint
  directory to the current (restart) directory
  """
  restart_dir_full = trick.command_line_args_get_output_dir()
  if '/' in trick.command_line_args_get_output_dir():
    restart_run_name = restart_dir_full.rpartition('/')[2]
  else:
    restart_run_name = restart_dir_full

  if '/' in checkpoint_dir:
    checkpoint_run_name = checkpoint_dir.rpartition('/')[2]
  else:
    checkpoint_run_name = checkpoint_dir

  import glob, shutil, subprocess
  chkptFiles = glob.glob('{0}/*chkpnt*'.format(checkpoint_dir))
  for chkptFile in chkptFiles:
    shutil.copy(chkptFile, '{0}'.format(restart_dir_full))
  p = subprocess.Popen('grep {0} {1}/{2}'.format(checkpoint_run_name, restart_dir_full, checkpoint_file), shell=True,
                       stdout = subprocess.PIPE, stderr = subprocess.PIPE, universal_newlines=True)
  res = p.communicate()
  lines = res[0].split('\n')[:-1]
  for line in lines:
     if line.startswith('trick_anon_local_'):
        allocName, _, varStr = line.split(' ')
        trailingChars = len(varStr.partition(checkpoint_run_name)[2])+5
        subprocess.check_call('sed -i"" -e "s/^char {0}\[.*/char {0}[{1}];/" {2}'.format(allocName, 
                              len(restart_dir_full)+trailingChars, checkpoint_file), shell=True, cwd='{0}'.format(restart_dir_full))
  p = subprocess.check_call('sed -i"" -e "s/{0}/{1}/g" *chkpnt*'.format(checkpoint_run_name, restart_run_name), shell=True, cwd='{0}'.format(restart_dir_full))
  p = subprocess.check_call('sed -i"" -e "s/char\* trick_inject_injector_executor_so_names.*//g" *chkpnt*', shell=True, cwd='{0}'.format(restart_dir_full))

  action = textwrap.dedent(string.Template("""
    trick.load_checkpoint("$dir/$file")
  """).substitute({'dir' : restart_dir_full, 'file' : checkpoint_file}))

  restart_event = trick.new_event("restart")
  restart_event.condition(0, "trick.exec_get_sim_time() == 0")
  restart_event.condition_all()
  restart_event.set_cycle(1)
  restart_event.action(0, action)
  restart_event.activate()

  trick.add_event(restart_event)

  return
