
all: kernel.img

include ../kernel/common.mk

include ../libvorbis/pkgconfig.mk
include ../libpng/pkgconfig.mk
include ../SDL2/pkgconfig.mk
include ../SDL2_image/pkgconfig.mk
include ../SDL2_mixer/pkgconfig.mk

OBJS = \
	source/lib/iters.o source/lib/readline.o \
	source/lib/utils/pyexec.o \
	source/py/argcheck.o \
	source/py/asmarm.o \
	source/py/asmthumb.o \
	source/py/asmx64.o \
	source/py/asmx86.o \
	source/py/bc.o \
	source/py/binary.o \
	source/py/builtinevex.o \
	source/py/builtinimport.o \
	source/py/compile.o \
	source/py/emitbc.o \
	source/py/emitcommon.o \
	source/py/emitglue.o \
	source/py/emitinlinethumb.o \
	source/py/emitnative.o \
	source/py/formatfloat.o \
	source/py/frozenmod.o \
	source/py/gc.o \
	source/py/lexer.o \
	source/py/lexerstr.o \
	source/py/lexerunix.o \
	source/py/malloc.o \
	source/py/map.o \
	source/py/modarray.o \
	source/py/modbuiltins.o \
	source/py/modcmath.o \
	source/py/modcollections.o \
	source/py/modgc.o \
	source/py/modio.o \
	source/py/modmath.o \
	source/py/modmicropython.o \
	source/py/modstruct.o \
	source/py/modsys.o \
	source/py/mpprint.o \
	source/py/mpstate.o \
	source/py/mpz.o \
	source/py/nativeglue.o \
	source/py/nlrsetjmp.o \
	source/py/obj.o \
	source/py/objarray.o \
	source/py/objattrtuple.o \
	source/py/objbool.o \
	source/py/objboundmeth.o \
	source/py/objcell.o \
	source/py/objclosure.o \
	source/py/objcomplex.o \
	source/py/objdict.o \
	source/py/objenumerate.o \
	source/py/objexcept.o \
	source/py/objfilter.o \
	source/py/objfloat.o \
	source/py/objfun.o \
	source/py/objgenerator.o \
	source/py/objgetitemiter.o \
	source/py/objint_longlong.o \
	source/py/objint_mpz.o \
	source/py/objint.o \
	source/py/objlist.o \
	source/py/objmap.o \
	source/py/objmodule.o \
	source/py/objnamedtuple.o \
	source/py/objnone.o \
	source/py/objobject.o \
	source/py/objpolyiter.o \
	source/py/objproperty.o \
	source/py/objrange.o \
	source/py/objreversed.o \
	source/py/objset.o \
	source/py/objsingleton.o \
	source/py/objslice.o \
	source/py/objstr.o \
	source/py/objstringio.o \
	source/py/objstrunicode.o \
	source/py/objtuple.o \
	source/py/objtype.o \
	source/py/objzip.o \
	source/py/opmethods.o \
	source/py/parse.o \
	source/py/parsenum.o \
	source/py/parsenumbase.o \
	source/py/qstr.o \
	source/py/repl.o \
	source/py/runtime.o \
	source/py/scope.o \
	source/py/sequence.o \
	source/py/showbc.o \
	source/py/smallint.o \
	source/py/stackctrl.o \
	source/py/stream.o \
	source/py/unicode.o \
	source/py/vm.o \
	source/py/vstr.o \
	source/py/warning.o \
	source/py/nlrthumb.o \
	source/py/nlrx64.o \
	source/py/nlrx86.o \
	source/py/nlrxtensa.o \
	source/sdlpi/mprun.o \
	source/sdlpi/mphalport.o \
	source/sdlpi/main.o \

	# source/microbit/help.o \
	# source/microbit/modlove.o \
	# source/microbit/modmicrobit.o \
	# source/microbit/modneopixel.o \
	# source/microbit/modrandom.o \
	# source/microbit/modthis.o \
	# source/microbit/mphalport.o \
	# source/microbit/modantigravity.o \
	# source/microbit/microbitaccelerometer.o \
	# source/microbit/microbitbutton.o \
	# source/microbit/microbitcompass.o \
	# source/microbit/microbitconstimage.o \
	# source/microbit/microbitconstimagetuples.o \
	# source/microbit/microbitdisplay.o \
	# source/microbit/microbiti2c.o \
	# source/microbit/microbitimage.o \
	# source/microbit/microbitmusic.o \
	# source/microbit/microbitmusictunes.o \
	# source/microbit/microbitpin.o \
	# source/microbit/microbitspi.o \
	# source/microbit/microbituart.o \
	# source/microbit/main.o \





#CFLAGS += -finline-functions -funswitch-loops -fpredictive-commoning -fgcse-after-reload -ftree-vectorize
CFLAGS += -O3 -Iinc -Iinc/sdlpi -Iinc/lib
#-ffreestanding -fvisibility=hidden -marm -mcpu=arm1176jzf-s -mfpu=vfp -mfloat-abi=softfp -D_REENTRANT -D__RASPBERRY_PI__ -DHAVE_USPI

-include $(DEPDIR)/*.Po


kernel.img: inc/genhdr/qstrdefs.generated.h $(OBJS) Makefile ../kernel/raspberry.ld $(LIBS_DEP)
	$(CXX) $(LDFLAGS) -o kernel.elf $(OBJS) $(LIBS)
	$(OBJCOPY) kernel.elf -O binary kernel.img
	rm kernel.elf

clean:
	rm -f $(OBJS) kernel.elf kernel.img kernel.map
	rm -r inc/genhdr/qstrdefs.generated.h
	rm -rf $(DEPDIR)
	rm -rf build


ECHO = @echo
PYTHON = python

QSTR_DEFS = inc/py/qstrdefs.h inc/sdlpi/qstrdefsport.h

# Note: we need to protect the qstr names from the preprocessor, so we wrap
# the lines in "" and then unwrap after the preprocessor is finished.
inc/genhdr/qstrdefs.generated.h: $(QSTR_DEFS) tools/makeqstrdata.py inc/sdlpi/mpconfigport.h inc/py/mpconfig.h
	$(ECHO) "Generating $@"
	mkdir -p build
	@cat $(QSTR_DEFS) | sed 's/^Q(.*)/"&"/' | $(CPP) -Iinc -Iinc/sdlpi - | sed 's/^"\(Q(.*)\)"/\1/' > build/qstrdefs.preprocessed.h
	@$(PYTHON) tools/makeqstrdata.py build/qstrdefs.preprocessed.h > $@
