#PURPOSE:
#   (This data file sets up the a pair interaction based on contact param names.)
#
#REFERENCE:
#   ((JSC Engineering Orbital Dynamics Models))
#
#ASSUMPTIONS AND LIMITATIONS:
#   ((?))
#
#


def set_contact_interaction(contact_reference) :

  pair_interaction_local = trick.SpringPairInteraction()
  pair_interaction_local.thisown = 0

  pair_interaction_local.params_1 = "steel"
  pair_interaction_local.params_2 = "steel"

  pair_interaction_local.spring_k = trick.attach_units( "lbf/in",20.0)
  pair_interaction_local.damping_b = trick.attach_units( "lbf*s/in",0.4)
  pair_interaction_local.mu = 0.05

  contact_reference.contact.register_interaction(pair_interaction_local)
