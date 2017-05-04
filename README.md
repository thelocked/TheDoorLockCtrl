# TheDoorLockCtrl

Release Note:
Denna release är baserad på att försöka få till den grund funktionalitet som först beskrevs att fungera någorlunda korrekt(ibland)
efter som det är mycket som "spökar" till och från. Samt att en del saker beter sig helt inkorrekt.
med förutsättning att endast "snäll" data matas in annars kan vad som helst hända.
Har ingen aning om vad som kan ske om man låter den snurra på en stund eller om hur många gånger man kan upprepa funktioner.

Ang KeyPad funktionaliteten så är den i nuvarande stadium helt bort kopplad, efter som denna lösning har någon annan knåpat ihop.
Jag tyckte dock att den teorin var snygg, men inte har haft någon möjlighet att testa om den fungerar och så att slippa att den blandar sig i denna soppa.

Beskrivning:
    bla bla.....

    Men ang att varför jag inkluderat ett bibliotek kallat "PinCode",så grundar sig detta på att jag från början trodde
    att jag kunde nyttja det bibliotek som Arduino själva tillhanda håller,
    Men det "sög fet" det vill säga omständligt, oflexibelt, samt gav användarna möjlighet att krascha hela systemet via det som matas in.
    Så till sist slutade det med att jag skrivit om det totalt från grunden.

Hur använder detta:
    För nuvarande så förlitar sig systemet på input/output av data och info via det serie com fönstret.
    I/O pinnarna kan bete sig lite hur som helst.

    När systemet startas så ska det komma ett meddelande när det är redo att ta emot data från användaren,
    men kan funka även om det inte visats.

    Det gör ingen skillnad om man väljer en pin kods siffra i taget eller kastar in en hel sträng och avlutande bekräftelse
    tex. "1234#"

    '#' tecken gör att det som har matats in verifieras med det som ska vara korrekt.
    '*' tecken nollställer och tar bort det som just matats in.(Dock inte räknaren för felaktiga försök)

    Systemet ger feeback på inmatningar som görs och om åtgärder vidtags. Dock finns det vissa saker som totalt ignoreras.

Hur får man detta att fungera:
    Följande filer måste ligga i en katalog med namnet "TheDoorLockCtrl" 
    TheDoorLockCtrl.ino
    PinCode.h
    PinCode.cpp
    OnewireKeypad.h
    BitBool.h

    Com hastigheten för seriella fönstret är satt till 9600

    Sedan ska det funka!(Kanske).






