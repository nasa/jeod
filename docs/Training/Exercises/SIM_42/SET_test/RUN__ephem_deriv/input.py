exec(compile(open("RUN__ephem_update__lunar_deriv/input.py", "rb").read(), "RUN__ephem_update__lunar_deriv/input.py", 'exec'))
dynamics.dyn_manager.deriv_ephem_update = True
trick.exec_set_job_onoff("moon.de4xx.propagate_lunar_rnp",1,0)
