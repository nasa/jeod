pair = trick.SpringPairInteraction()
pair.thisown = 0
pair.params_1 = "steel"
pair.params_2 = "steel"
pair.spring_k = trick.attach_units( "N/m",100.0)
pair.damping_b = trick.attach_units("N*s/m",20.0)
pair.mu = 0.4

interactions.contact.register_interaction(pair)
