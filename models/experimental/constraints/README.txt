The enclosed model attempts to model the dynamic properties of a fluid such as
a fuel inside a rigid tank.  There is currently no known S_define that integrates
the various source code files in a structured fashion nor a working input file.
However the following notes have been captured to attempt to help a user who
might want to work with this model to create a working code base.

- Ideal source for the physics related to this is “The New “Dynamic Behavior of
 Liquids In Moving Containers””, Dodge, Franklin T., Southwest Research Institute,
 2000.

- The enclosed model attempts to provide an analytical estimate of the reaction
 forces created as a simple fluid moves in a tank.  Multiple tanks can be defined.
 The goal appears to be that the model provides the tank constraints and have the
 fluid behaving as a pendulum within those constraints.

- Note that the pendulum properties must change as a function of fluid level in
 particular.  Other models such as the ANTARES vehicle slosh model use the above
 reference as a mechanism to create tables that are a function of fluid fill level
 and other parameters rather than include analytical methodology for computing
 pendulum model change.

- Glenn Research Center staff was invaluable in assisting with a basic understanding
 of the structure of the ANTARES model as well fundamental slosh model theory,
 providing the Dodge reference as well as other ANTARES model references not
 included here.

Note on StructureIntegratedDynBody:
This specialization of DynBody is designed to reflect structures which are not
entirely rigid. The specific cases include rigid structures with one or more
mobile masses which undergo constrained motion relative to the rigid structure.
This flavor of DynBody differs in that the coordinate frame of the rigid body
is integrated rather than the center of mass frame. This difference introduces
some mathematical complexities which are beyond the scope of this document. One
common use case for this model is to implement the dynamics of fluid such as
fuel moving in partially full tanks. A slosh model example is included.
