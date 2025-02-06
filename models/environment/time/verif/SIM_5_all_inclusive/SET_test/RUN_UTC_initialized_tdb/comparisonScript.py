import math

tai_tjt_at_epoch = 11544.499627500    # J2000 Jan 1, 2000::11:59:27.816
tdb_tjt_at_epoch = 11544.499999999159 # TDB is 7.2659E-5 s behind TT at J2000
tt_tjt_at_epoch  = 11544.5            # J2000, Jan 1, 2000:: 12:00:00.0

class Time :

    def __init__ (self, tjt_at_epoch) :
        self.tjt_at_epoch = tjt_at_epoch
        self.set_time_by_seconds (0.0)

    def set_time_by_seconds (self, seconds) :
        self.seconds = seconds
        self.trunc_julian_time = \
            self.tjt_at_epoch + self.seconds/86400.0


class TimeConverter_TAI_TDB :

    def __init__(self) :
        global tai
        global tdb
        self.tai_to_tt_offset = 32.184
        self.a_to_b_offset_epoch = \
            (tdb.tjt_at_epoch - tai.tjt_at_epoch) * 86400.0 \
            - self.tai_to_tt_offset
        self.prev_tai_seconds = 0.0
        self.prev_tdb_seconds = 0.0
        self.a_to_b_offset = 0.0

        # For debugging only.
        self.ncalls = 0
        self.niter = 0
        self.max_steps = 0

    def _set_a_to_b_offset(self) :
        global tai
        g = math.pi / 180.0 * (
            357.53 + 0.9856003 * (tai.trunc_julian_time - tai.tjt_at_epoch))
        self.a_to_b_offset = 0.001658*math.sin(g) + 0.000014*math.sin(2.0*g)

    def convert_a_to_b (self) :
        global tai
        global tdb
        self._set_a_to_b_offset()
        tdb.set_time_by_seconds (
            tai.seconds + self.a_to_b_offset - self.a_to_b_offset_epoch)

    def convert_b_to_a (self) :
        global tai
        global tdb
        # Initial guess.
        tai.set_time_by_seconds (
            self.prev_tai_seconds + (tdb.seconds - self.prev_tdb_seconds))
        nsteps = 0
        while True :
            nsteps += 1
            self.niter += 1
            self._set_a_to_b_offset()
            dtai = (tdb.seconds - tai.seconds) \
                - (self.a_to_b_offset - self.a_to_b_offset_epoch)
            tai.set_time_by_seconds (tai.seconds + dtai)
            if nsteps > 5 or abs(dtai/tai.seconds) < 1e-15 :
                break
        self.prev_tdb_seconds = tdb.seconds
        self.prev_tai_seconds = tai.seconds
        self.max_steps = max(self.max_steps, nsteps)
        self.ncalls += 1


tai = Time(tai_tjt_at_epoch)
tdb = Time(tdb_tjt_at_epoch)
tai_to_tdb = TimeConverter_TAI_TDB()

# Initial time to TAI 01:23:45.0 on 2017 day 301 #to delete
# Initial time to TAI 15:00:00.0 on 2018 day 157_ 
tai.set_time_by_seconds (
    00.0+60.0*(2:3.0+60.0*01.0)+86400.0*(300.0+17*365.0+5.0))
tai_to_tdb.convert_a_to_b()

tdb.set_time_by_seconds (tdb.seconds + 60.0)
tai_to_tdb.convert_b_to_a()
print("Test 1: delta TAI (increment TDB by 1 minute) = {:g}".format(
    tai.seconds - prev_tai - 61.0))

tdb.set_time_by_seconds (tdb.seconds + 1800.0)
tai_to_tdb.convert_b_to_a()
print("Test 2: delta TAI (increment TDB by 30 minutes) = {:g}".format(
    tai.seconds - prev_tai - 1799.0))

###########
