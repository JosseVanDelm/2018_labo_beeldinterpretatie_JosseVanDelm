# Gebruik code

Dit programma voer local feature matching uit. Een template wordt gematcht op een matching image.

Gebruik volgende argumenten bij het programma:
* `--matching_image=` met het absolute pad naar de afbeelding waarop een template gevonden moet worden.
* `--template_image=` met het absolute pad naar de afbeelding die gevonden moet worden in de "matching image".
* gebruik `--help` als je de voornaam van je moeder vergeten bent. Ga dan best ook even langs een dokter.

Gebruik eender welke toets om door het programma te gaan.

Hieronder staat voor de volledigheid nog eens de opdracht:

# Sessie 3 - keypoint detection and matching

Met template matching gaan we op zoek naar een exact object. Wanneer er echter een grote vervorming van het gekende object kan optreden, dan lijkt template matching al snel te falen. In dit labo voeren we een analyse uit van een eyetracker opname waarbij we op zoek gaan naar een vooraf bepaald object (kinderbueono and fitness cornflakes).

* **Opdracht 1**: Bepaal op de inputbeelden en op de te zoeken templates keypoints, gebaseerd op de volgende keypoint types: ORB, BRISK en AKAZE.
* **Opdracht 2**: Kies 1 van bovenstaande keypoint detectoren, bepaal descriptoren van de gegenereerde keypoints en doe een brute force matching tussen beide sets.
* **Opdracht 3**: Pas RANSAC toe om enkel de keypoint matches over te houden die op iets slaan. Gebruik de resulterende matches om een homography uit te rekenen en het object in uw doelbeeld te vinden.
