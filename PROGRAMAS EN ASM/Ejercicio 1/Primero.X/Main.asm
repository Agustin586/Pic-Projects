#INCLUDE <P16F84A.INC>    ;INLCUYE LA LIBRERIA
LIST P=16F84A	;Procesador
    
ORG 0	;Incio
    BSF	STATUS,5    ;Banco 1
    BCF	TRISB,6	    ;Pin 6 como salida
    BCF	TRISB,2	    ;Pin 2 como salida
    BSF	TRISA,3	    ;Pin 2 como entrada
    BCF	STATUS,5    ;De vuelta Banco 0
    GOTO    VERIFICA
    
VERIFICA:
    BTFSS   PORTA,3 ;RA3 == 1 ?
    GOTO    APAGAR  ;VOY A APAGAR
    GOTO    INICIO
    
APAGAR:
    BCF	PORTB,6	    ;APAGO PIN 6
    BCF	PORTB,2	    ;APAGO PIN 2
    GOTO    VERIFICA	;VOY A VERIFICAR
    
INICIO:
    BSF PORTB,6	    ;Pongo '1' pin 6 del puerto B
    BSF	PORTB,2	    ;RB2 = 1
    GOTO    VERIFICA
END
    


