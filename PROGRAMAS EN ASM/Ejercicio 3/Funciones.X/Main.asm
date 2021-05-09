#INCLUDE    <P16F84A.INC>
LIST P=16F84A
    
ORG 0
    BSF	STATUS,5
    MOVLW   B'00010001'
    MOVWF   TRISA
    CLRF    TRISB
    BCF STATUS,5
    CLRF    PORTB
    GOTO BUCLE
    ;===============
    
BUCLE:
    CALL    LED1	;PRENDE B0
    CALL    LED2	;VERIFICA A4
    CALL    APAGAR	;FUNCION APAGAR LEDSGOTO    VERF2	;VERIFICA A4
    GOTO    BUCLE	;BUCLE INFINITO
    
LED1:
    BTFSC   PORTA,0	;A0=0?
    BSF	    PORTB,0	;PONE LED1=1
    RETURN		;RETORNA LA PILA
    
LED2:
   BTFSC    PORTA,4	;A4=0?
   BSF	    PORTB,1	;LED2=1
   RETURN		;RETORNA LA PILA
   
APAGAR:
    BTFSS   PORTA,0	;A0=1?
    BCF	    PORTB,0	;LED1=0
    BTFSS   PORTA,4	;A4=1?
    BCF	    PORTB,1	;LED2=0
    RETURN
END

