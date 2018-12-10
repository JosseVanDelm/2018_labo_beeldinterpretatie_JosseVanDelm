# Gebruik van de code
Dit deel bestaat uit twee programma's
* `sessie_6_0` voor Opdracht 1 en 2
* `sessie_6_1` voor Opdracht 3

Algemene opmerkingen
* Je gebruikt best absolute paden in codeblocks
* Je kan de filmpjes vroegtijdig afsluiten door op `ESC` te drukken.

# sessie_6_0: Gezichtsdetectie
Programma heeft volgende argumenten:
* `--help` toont de help
* `--video` stelt het pad in voor de video
* `--model` stelt het pad in naar een xml-file die een cascade classifier bepaalt.
* `--model2` (*optioneel*) stelt het pad in naar een tweede xml-file die een cascade classifier bepaalt. Indien dit pad is ingesteld worden de twee classifiers toegepast op de gegeven video.

# sessie_6_1: Voetgangerdetectie
Programma heeft volgende argumenten:
* `--help` toont de help
* `--video` stelt het pad in voor de video



# Sessie 6 - Object Detection

In dit labo gaan we 2 verschillende soorten object detectors gebruiken om eerst gezichten te vinden. Daarna gebruiken we een andere detector om personen te vinden.

* **Opdracht 1**: Maak een interface waarin je de fases.mp4 video kan afspelen, daarna gebruik je het Viola en Jones detectie algoritme om met het HAAR face model gezichten in elk frame te vinden. Duid deze aan met een cirkel en een detectie score.

* **Opdracht 2**: Detecteer nu niet enkel met het HAAR face model, maar ook het LBP model. Wat merk je van verschillen?

* **Opdracht 3**: Maak nu een ander project waarin we dezelfde interface hebben, maar met de people.mp4 video. Hierin moet je personen vinden gebruikmakende van de HOG+SVM pedestrian detector.

* **EXTRA**: Kan je ook per persoon zijn/haar traject in de video bijhouden?
