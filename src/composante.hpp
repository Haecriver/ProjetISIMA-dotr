/*!
 * \file main.cpp
 * \brief Declaration de l'objet composante
 * \author Chevalier_Pierre
 * \version 0.1
 */
 
#ifndef COMPOSANTE_HPP
#define COMPOSANTE_HPP

#include <opencv2/highgui/highgui.hpp>
#include <vector>

using namespace cv;

/*!\class Simulation
 * \brief classe contenant tous les points d'une composante connexe d'une image	
 */
class Composante{
	private:		
		std::vector<Point> 	_points; 	/*!< Liste des points de la composante */
		Point 				_pos;		/*!< Position (calculee) de la composante */
		
		unsigned			_height;		/*!< Hauteur calculee de la composante 
											(point le + haut - point le plus bas) */
		unsigned			_width;		/*!< Largeur calculee de la composante 
											(point le plus a droite - point le plus a gauche) */
		Point				_baseline;	/*!< Point ou commence la forme */
											
		static unsigned const MAX_BINARY_VALUE = 255;
		
		/*!
		 *  \brief Calculateur des attributs de la classe
		 *
		 *  Recalcul les moyenne et taille de la composante courante
		 */
		void computeAtt();
		
		/*!
		 *  \brief Calcul un composant a partir d'un point donne
		 *
		 *  (FONCTION RECURSIVE)
		 *  Ajout le point donne en parametre au composant, et va verifie les points voisins.
		 *  Si ces points voisins sont aussi elligible, on les rajoute au composant, on passe le
		 *  pixel courant a 0 et on verifie les nouveaux voisins.
		 *
		 *  \param copy : Reference de la copie d'image, dont les pixels vont etre passe a 0 une fois traite
		 *  \param pointCur : Point courant a traiter
		 */
		void computeAComposante(Mat& copy, Point pointCur);
		
	public:
	
		/*!
		 *  \brief Constructeur
		 *
		 *  Constructeur de la classe Composante.
		 */
		Composante();
		
			
		/*!
		 *  \brief Constructeur
		 *
		 *  Constructeur de la classe Composante avec parametre.
		 *
		 *  \param copy : Reference de la copie d'image, dont les pixels vont etre passe a 0 une fois traite
		 *  \param pointCur : Point courant a traiter
		 */
		Composante(Mat& copy, Point pointCur);
		
		/*!
		 *  \brief Construction de vecteur composante
		 *
		 *  Creer un vecteur de composante contenant toutes les composantes connexes
		 *  de l'image src passee en parametre.
		 *  L'image en parametre n'est pas affectee par cette fonction.
		 *
		 *  \param src : Reference constante vers l'image a traiter.
		 *  \return Le vecteur de composantes connexes de l'image.
		 */
		static std::vector<Composante> getComposantes(const Mat& src, unsigned max_comp);
			
		/*!
		 *  \brief Ajout de point
		 *
		 *  \param point : Point a ajouter a la composante.
		 */
		void addPoint(Point point);
		
		/*!
		 *  \brief Getter position
		 */
		Point& getPosition(){
			return _pos;
		}
		
		unsigned getWidth(){
			return _width;
		}
		
		unsigned getHeight(){
			return _height;
		}
};

#endif
