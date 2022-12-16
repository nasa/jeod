"""
"  Attach
"""
# Attach payload primary to vehicle
payload.attach_primary_to_veh.action_name = "attach_payload_to_veh"
payload.attach_primary_to_veh.dyn_subject = payload.primary_body
payload.attach_primary_to_veh.dyn_parent  = veh.dyn_body
payload.attach_primary_to_veh.subject_point_name = "vehicle_attach"
payload.attach_primary_to_veh.parent_point_name  = "payload_attach"
payload.attach_primary_to_veh.active = True                                
dynamics.dyn_manager.add_body_action( payload.attach_primary_to_veh )
                                
# Attach payload secondary to primary
payload.attach_secondary_to_primary.action_name = "add_mass_to_payload"
payload.attach_secondary_to_primary.subject     = payload.secondary_mass
payload.attach_secondary_to_primary.dyn_parent  = payload.primary_body
payload.attach_secondary_to_primary.subject_point_name = "attach_point"
payload.attach_secondary_to_primary.parent_point_name  = "other_node"
payload.attach_secondary_to_primary.active = True
dynamics.dyn_manager.add_body_action( payload.attach_secondary_to_primary )

