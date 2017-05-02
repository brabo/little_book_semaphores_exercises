CC=gcc

CFLAGS=-I. --std=gnu99 -I./include -pthread


WAIT_TEST_OBJ=src/wait_test.o
BASIC_SIG_OBJ=src/basic_sig.o
BASIC_RDV_OBJ=src/basic_rdv.o
BASIC_RDV2_OBJ=src/basic_rdv2.o src/bar.o src/sem.o
BASIC_MTX_OBJ=src/basic_mtx.o
BASIC_MUX_OBJ=src/basic_mux.o
BASIC_MUX2_OBJ=src/basic_mux2.o src/mux.o
BASIC_BAR_OBJ=src/basic_bar.o
BASIC_REBAR_OBJ=src/basic_rebar.o
BASIC_PRETURN_OBJ=src/basic_preturn.o
BASIC_BAROO_OBJ=src/basic_baroo.o
BASIC_BAROO2_OBJ=src/basic_baroo2.o src/bar.o src/sem.o
BASIC_XQUEUE_OBJ=src/basic_xqueue.o src/bar.o src/sem.o
BASIC_XQUEUE2_OBJ=src/basic_xqueue2.o src/xq.o src/bar.o src/sem.o
PROCON_OBJ=src/procon.o src/rb.o src/sem.o
PROCONM_OBJ=src/procon-multi.o src/rb.o src/sem.o
PHIL_OBJ=src/dining_phil.o
SMOKE_OBJ=src/smokers.o
SAVAGE_OBJ=src/savages.o

LDFLAGS=
LDLIBS=


wait_test: $(WAIT_TEST_OBJ)
	$(CC) -o $@ $(WAIT_TEST_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(WAIT_TEST_OBJ)

basic_sig: $(BASIC_SIG_OBJ)
	$(CC) -o $@ $(BASIC_SIG_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(BASIC_SIG_OBJ)

basic_rdv: $(BASIC_RDV_OBJ)
	$(CC) -o $@ $(BASIC_RDV_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(BASIC_RDV_OBJ)

basic_rdv2: $(BASIC_RDV2_OBJ)
	$(CC) -o $@ $(BASIC_RDV2_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	##rm -f $(BASIC_RDV2_OBJ)

basic_mtx: $(BASIC_MTX_OBJ)
	$(CC) -o $@ $(BASIC_MTX_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(BASIC_MTX_OBJ)

basic_mux: $(BASIC_MUX_OBJ)
	$(CC) -o $@ $(BASIC_MUX_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(BASIC_MUX_OBJ)

basic_mux2: $(BASIC_MUX2_OBJ)
	$(CC) -o $@ $(BASIC_MUX2_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(BASIC_MUX2_OBJ)

basic_bar: $(BASIC_BAR_OBJ)
	$(CC) -o $@ $(BASIC_BAR_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(BASIC_BAR_OBJ)

basic_rebar: $(BASIC_REBAR_OBJ)
	$(CC) -o $@ $(BASIC_REBAR_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(BASIC_REBAR_OBJ)

basic_preturn: $(BASIC_PRETURN_OBJ)
	$(CC) -o $@ $(BASIC_PRETURN_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(BASIC_PRETURN_OBJ)

basic_baroo: $(BASIC_BAROO_OBJ)
	$(CC) -o $@ $(BASIC_BAROO_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(BASIC_BAROO_OBJ)

basic_baroo2: $(BASIC_BAROO2_OBJ)
	$(CC) -o $@ $(BASIC_BAROO2_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	##rm -f $(BASIC_BAROO2_OBJ)

basic_xqueue: $(BASIC_XQUEUE_OBJ)
	$(CC) -o $@ $(BASIC_XQUEUE_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	##rm -f $(BASIC_XQUEUE_OBJ)

basic_xqueue2: $(BASIC_XQUEUE2_OBJ)
	$(CC) -o $@ $(BASIC_XQUEUE2_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(BASIC_XQUEUE2_OBJ)

procon: $(PROCON_OBJ)
	$(CC) -o $@ $(PROCON_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	##rm -f $(PROCON_OBJ)

proconm: $(PROCONM_OBJ)
	$(CC) -o $@ $(PROCONM_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(PROCONM_OBJ)

phil: $(PHIL_OBJ)
	$(CC) -o $@ $(PHIL_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(PHIL_OBJ)

smoke: $(SMOKE_OBJ)
	$(CC) -o $@ $(SMOKE_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(SMOKE_OBJ)

savage: $(SAVAGE_OBJ)
	$(CC) -o $@ $(SAVAGE_OBJ) $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	rm -f $(SAVAGE_OBJ)

all: wait_test basic_sig basic_rdv basic_rdv2 basic_mtx basic_mux basic_mux2 basic_bar basic_rebar basic_preturn basic_baroo basic_baroo2 basic_xqueue basic_xqueue2 procon proconm phil smoke savage

clean:
	rm -f src/*.o wait_test basic_sig basic_rdv basic_rdv2 basic_mtx basic_mux basic_mux2 basic_bar basic_rebar basic_preturn basic_baroo basic_baroo2 basic_xqueue basic_xqueue2 procon proconm phil smoke savage
