# Para compilar o programa para correr sem LEDs e com sensor dummy (PC ou Raspberry):
#      make trabalhofinal
#
# Para compilar o programa para correr com LEDs e sensor (só Raspberry):
#      make AAC_RASPBERRY=1 trabalhofinal 
# 
# Para compilar o modo debug (inserir valores):
#	   make debug


CFLAGS = -Wall
LDFLAGS = -lm

ifdef AAC_RASPBERRY
LDFLAGS += -lwiringPi
CFLAGS += -DAAC_RASPBERRY=$(AAC_RASPBERRY)
endif

trabalhofinal: trabalhofinal.c
	gcc $(CFLAGS) -o trabalhofinal  trabalhofinal.c $(LDFLAGS) 

clean: 
	rm trabalhofinal

edit:
	nano trabalhofinal.c

debug:
	gcc $(CFLAGS) -o debug debug.c $(LDFLAGS)
	./debug