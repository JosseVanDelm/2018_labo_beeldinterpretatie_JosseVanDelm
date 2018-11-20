# Gebruik Code

***DEZE CODE WERKT MOMENTEEL NOG NIET!!!***
De fout is aangegeven in de code in commentaar, zie ook `FIXME`.
Ik probeer een mask toe te passen op de afbeelding die bekomen wordt door correlatie, maar dit werkt niet:

```
OpenCV(3.4.1) Error: Sizes of input arguments do not match (The operation is neither 'array op array' (where arrays have the same size and type), nor 'array op scalar', nor 'scalar op array') in binary_op, file /builddir/build/BUILD/opencv-3.4.1/modules/core/src/arithm.cpp, line 225
terminate called after throwing an instance of 'cv::Exception'
  what():  OpenCV(3.4.1) /builddir/build/BUILD/opencv-3.4.1/modules/core/src/arithm.cpp:225: error: (-209) The operation is neither 'array op array' (where arrays have the same size and type), nor 'array op scalar', nor 'scalar op array' in function binary_op

Process returned -1 (0xFFFFFFFF)   execution time : 10.267 s
Press ENTER to continue. 
```

De lijnnummers die weergegeven zijn in errormessage kunnen veschillen, maar het komt neer op de lijn met het `FIXME` statement.

Het gebruik van de code is heel simpel, je kan volgende argumenten gebruiken:
* `--help` om de help te laten zien.
* `--matching_image` om het (best absolute) pad naar de afbeelding in te stellen waarop aan template matching wordt gedaan.
* `--template` om de template (ook best via absoluut pad!) in te stellen.
Hieronder is nog eens een kopie van de oorspronkelijke opdracht:

# Sessie 3 - template based matching

Kleur segmentatie en connected components brengen je maar tot een beperkte oplossing bij heel wat problemen. Daarom gaan we deze week een stap verder. Op een transportband in een industriele setup komen zowel metalen plaatjes als metalen ringen voorbij (zie input foto). De metalen plaatjes moeten gedetecteerd worden en gelabeld als in de output foto, zodanig dat een autonome robot deze van de band kan plukken. Hiervoor zullen we gebruik maken van template matching.

* Opdracht 1: Gebruik template matching om een object te vinden in een inputbeeld
* Opdracht 2: Pas de template matching aan om lokaal naar maxima te zoeken, zodanig dat je alle matches vind
* Extra: Pas de template matching aan geroteerde objecten te vinden (roteren van beeld, rotatedRect, warpPerspective op hoekpunten)

Tip voor extra opdracht: https://github.com/OpenCVBlueprints/OpenCVBlueprints/blob/master/chapter_5/source_code/rotation_invariant_detection/rotation_invariant_object_detection.cpp


