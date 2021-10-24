const   screenWidth = 250, //Les dimension de l'ecran en Pixel
        screenHeight = 250;

const   gridWidth = 11,     //Le nombre de lignes de la grille     
        gridHeight = 11;
        /*NOTE: si tu veux jouer avec la précision, change les valeurs gridWidth 
        et gridHeight à 6, 27 ou bien 51
        */


        //les dimension d'une case de la grille
const   cellWidth = screenWidth / (gridWidth - 1),
        cellHeight = screenHeight / (gridHeight - 1);



const   facteur = 500 / screenHeight; //ca c'est juste pour l'affichage

//la fonction main
window.onload =  function () {


    //Ca c'est pour afficher sur la page web
    var canvas = document.getElementById('canvas'),
        context = canvas.getContext('2d');


    //On crée une matrice 2d représentant l'ecran de 600px par 600px
    var screen = newMatrice_2d(screenWidth, screenHeight);
    
    //On gènere une grille 2d (une matrice, c'est expliqué dans le code de la fonction) 
    var grille = generateGrid(gridWidth, gridHeight);



    for (let i = 0; i < screen.length; i++) 

        for (let j = 0; j < screen[i].length; j++) 
        {

            let distanceVectors = new Array( 4 ),
                produitsArray = new Array( 4 );

            distanceVectors[0] = {
                x: j % cellWidth,
                y: i % cellHeight,
            }

            distanceVectors[1] = {
                x: j % cellWidth - cellWidth,
                y: i % cellHeight,
            }

            distanceVectors[2] = {
                x: j % cellWidth,
                y: i % cellHeight - cellHeight,
            }

            distanceVectors[3] = {
                x: j % cellWidth - cellWidth,
                y: i % cellHeight - cellHeight,
            }

            //===================================
            let gridPos = {
                x: Math.floor( j / cellWidth ),
                y: Math.floor( i /  cellHeight )
            }



            produitsArray[0] = produitScalaire(
                distanceVectors[0],
                grille[ gridPos.y ][ gridPos.x ]
            )
            produitsArray[1] = produitScalaire(
                distanceVectors[1],
                grille[ gridPos.y ][ gridPos.x + 1 ]
            )
            produitsArray[2] = produitScalaire(
                distanceVectors[2],
                grille[ gridPos.y + 1 ][ gridPos.x ]

            )
            produitsArray[3] = produitScalaire(
                distanceVectors[3],
                grille[ gridPos.y +1 ][ gridPos.x + 1]
            )

            //===================================

            var zebi_1 = interpolate( 
                produitsArray[0], 
                produitsArray[1],
                distanceVectors[0].x * (1/cellWidth)
            );

            var zebi_2 = interpolate(
                produitsArray[2], 
                produitsArray[3],
                distanceVectors[0].x  * (1/cellWidth)      
            );

            var zebi_total = interpolate(
                zebi_1, 
                zebi_2,
                distanceVectors[0].y  * (1/cellHeight)    
            )
            
            //on dessine
            let val1 = zebi_total * 30 + 120;
            let val2 = zebi_total * 10 + 120;
            let val3 = Math.atan(zebi_total) * 80 + 125;

            context.fillStyle = "rgb(" + val2 + ',0, 0)';
            context.fillRect(j*facteur, i*facteur, facteur, facteur);
     
        }
        


}

function newMatrice_2d(w, h) {
    var matrice = new Array(w);

    for (let i = 0; i < matrice.length; i++)

        matrice[i] = new Array(h).fill(0);

    return matrice

}

function generateGrid(gridWidth, gridHeight){

    //On crée une matrice 2d, qu'on init à 0
    var grille = newMatrice_2d( gridWidth, gridHeight )

    
    for (let i = 0; i < grille.length; i++) {

        for (let j = 0; j < grille[i].length; j++) {


            //A chaque point de la grille, on assigne un veteur dont la longueur est 1
            //et aussi dont l'angle est aléatoire

            var randomAngle = Math.random() * Math.PI * 2  ;

            grille[i][j] = {
                x : Math.cos( randomAngle ),

                y : Math.sin( randomAngle )
            };
            
        }
    }

        
    
    return grille;

}

function produitScalaire(vectA, vectB){

    return vectA.x * vectB.y + vectA.y * vectB.x;

}

function normalizeVector(vect) {
    let angle = Math.atan(vect.y/vect.x);

    return {
        x: Math.cos(angle),
        y: Math.sin(angle),
    }
}

//===================================================

//Smooth step est une fonction qui existe déja
//Genre prédéfinie, https://en.wikipedia.org/wiki/Perlin_noise

function interpolate( a0,  a1,  w) {
    //default
    //return (1.0 - w)*a0 + w*a1;

    // Use this cubic interpolation [[Smoothstep]] instead, for a smooth appearance:
    return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
    
    // Use [[Smootherstep]] for an even smoother result with a second derivative equal to zero on boundaries:
    //return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
    
}
