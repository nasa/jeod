#!/usr/bin/env perl

use strict;

if(!defined $ENV{'JEOD_HOME'}) {
    die("JEOD_HOME is undefined");
}
my $home = $ENV{JEOD_HOME};
my $scripts = "$home/dgh_scripts";
my $template = "$scripts/pdf_config.dox";


# Model names
my %model_name = (
   AERODYNAMICS      => 'Aerodynamics Model',
   ATMOSPHERE        => 'Atmosphere Model',
   BODYACTION        => 'Body Action Model',
   CONTACT           => 'Contact Model',
   CONTAINER         => 'Container Model',
   DERIVEDSTATE      => 'Derived State Model',
   DYNBODY           => 'Dynamic Body Model',
   DYNMANAGER        => 'Dynamics Manager Model',
   EARTHLIGHTING     => 'Earth Lighting Model',
   EPHEMERIDES       => 'DE4xx Solar System Ephemerides',
   GRAVITY           => 'Gravity Model',
   GRAVITYTORQUE     => 'Gravity Gradient Torque Model',
   INTEGRATION       => 'Integration Routines',
   LVLHFRAME         => 'LVLH Frame Model',
   MASS              => 'Mass Tree Model',
   MATH              => 'Mathematical Functions',
   MEMORY            => 'Memory Allocation Routines',
   MESSAGE           => 'Message Handling Class',
   MODELTEMPLATE     => 'Model Template Model',
   SIMINTERFACE      => 'Simulation Interface Macro',
   NAMEDITEM         => 'Named Item Routines',
   ORBITALELEMENTS   => 'Orbital Elements Model',
   ORIENTATION       => 'Orientation Model',
   PLANET            => 'Planet Model',
   PLANETFIXED       => 'Planet Fixed Model',
   QUATERNION        => 'Quaternion',
   RADIATIONPRESSURE => 'Radiation Pressure Model',
   REFFRAMES         => 'Reference Frame Model',
   RELKIN            => 'Relative Kinematics Computations Model',
   RNP               => 'Rotation, Nutation, and Precession Model',
   SPICE             => 'Spice Model',
   SURFACEMODEL      => 'Surface Model',
   THERMALRIDER      => 'Thermal Model',
   TIME              => 'Time Model'
);

# Model paths
my %model_path = (
   AERODYNAMICS      => 'models/interactions/aerodynamics',
   ATMOSPHERE        => 'models/environment/atmosphere',
   BODYACTION        => 'models/dynamics/body_action',
   CONTACT           => 'models/interactions/contact',
   CONTAINER         => 'models/utils/container',
   DERIVEDSTATE      => 'models/dynamics/derived_state',
   DYNBODY           => 'models/dynamics/dyn_body',
   DYNMANAGER        => 'models/dynamics/dyn_manager',
   EARTHLIGHTING     => 'models/environment/earth_lighting',
   EPHEMERIDES       => 'models/environment/ephemerides',
   GRAVITY           => 'models/environment/gravity',
   GRAVITYTORQUE     => 'models/interactions/gravity_torque',
   INTEGRATION       => 'models/utils/integration',
   LVLHFRAME         => 'models/utils/lvlh_frame',
   MASS              => 'models/dynamics/mass',
   MATH              => 'models/utils/math',
   MEMORY            => 'models/utils/memory',
   MESSAGE           => 'models/utils/message',
   MODELTEMPLATE     => 'models/utils/model_template',
   SIMINTERFACE      => 'models/utils/sim_interface',
   NAMEDITEM         => 'models/utils/named_item',
   ORBITALELEMENTS   => 'models/utils/orbital_elements',
   ORIENTATION       => 'models/utils/orientation',
   PLANET            => 'models/environment/planet',
   PLANETFIXED       => 'models/utils/planet_fixed',
   QUATERNION        => 'models/utils/quaternion',
   RADIATIONPRESSURE => 'models/interactions/radiation_pressure',
   REFFRAMES         => 'models/utils/ref_frames',
   RELKIN            => 'models/dynamics/rel_kin',
   RNP               => 'models/environment/RNP',
   SPICE             => 'models/environment/spice',
   SURFACEMODEL      => 'models/utils/surface_model',
   THERMALRIDER      => 'models/interactions/thermal_rider',
   TIME              => 'models/environment/time',
);


foreach my $key (keys %model_path) {
   chdir $home;
   system "rm -rf doxygen";

   my $path = $model_path{$key};
   my $name = $model_name{$key};

   # chdir $path or next;
   mkdir 'doxygen' unless -e 'doxygen';
   system "sed -e 's/INSERT_MODEL_NAME_HERE/$name/' $template > doxygen/config.dox"
      and die;
   open FIND, "find $path -name '*.[ch][ch]'|" .
              "grep -v '/verif/'|" .
              "sed -e 's/^\\.\\///'|"
      or next;
   my @files = <FIND>;
   close FIND;
   next unless scalar @files; # skip if no .cc or .hh files found
   map chomp, @files; # removing trailing file extension

   foreach my $file (@files) {
      $file =~ /^(.*)\/([^\/]+)/ or die; # file must be nested: example.cc dies. submodel/example.cc succeeds
      my $dox_path = "doxygen/$1";
      unless (-d $dox_path) {
         system "mkdir -p $dox_path" and do {
            die "Can't create directory $dox_path";
         };
      }
      system "cp $file doxygen/$file" and do {
         die "Can't copy $file to doxygen/$file";
      };
   }

   chdir 'doxygen' or next;
   system "doxygen config.dox" and next;
   chdir 'doxygen/latex' or next;
   system 'make';
   system "cp refman.pdf $home/$path/docs/refman.pdf" if (-f 'refman.pdf');
   chdir '../../..';
   system "rm -rf doxygen";
}
