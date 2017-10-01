/********************************************************************************\
**                                                                              **
**  Copyright (C) 2014 Harijs Grinbergs                                         **
**                                                                              **
**  This file is a part of the ENIGMA Development Environment.                  **
**                                                                              **
**                                                                              **
**  ENIGMA is free software: you can redistribute it and/or modify it under the **
**  terms of the GNU General Public License as published by the Free Software   **
**  Foundation, version 3 of the license or any later version.                  **
**                                                                              **
**  This application and its source code is distributed AS-IS, WITHOUT ANY      **
**  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   **
**  FOR A PARTICULAR PURPOSE. See the GNU General Public License for more       **
**  details.                                                                    **
**                                                                              **
**  You should have recieved a copy of the GNU General Public License along     **
**  with this code. If not, see <http://www.gnu.org/licenses/>                  **
**                                                                              **
**  ENIGMA is an environment designed to create games and other programs with a **
**  high-level, fully compilable language. Developers of ENIGMA or anything     **
**  associated with ENIGMA are in no way responsible for its users or           **
**  applications created by its users, or damages caused by the environment     **
**  or programs made in the environment.                                        **
**                                                                              **
\********************************************************************************/

#ifndef MATHEM_FUNC_H
#define MATHEM_FUNC_H

//#include <math.h>
#include <cmath>
#include <vector>
#include <algorithm>    // std::fill

using std::vector;
//using std::abs;

namespace enigma_user {

template <class T>
class matrix
{
    public:
        int m_width;
        int m_height;
        vector< vector<T> > m_data;

        matrix<T> ():m_width(0),m_height(0){ };
        /**
         * @brief Copy constructor
         */
        matrix<T> (const matrix &mSource)
        {
            m_width = mSource.m_width;
            m_height = mSource.m_height;
            m_data = mSource.m_data;
        }

        /**
         * @brief Constructor for matrix of size width x height. Values are default type constructor values.
         *
         * @param width Matrix width (columns)
         * @param height Matrix height (rows)
         * @return Matrix of size width x height
         */
        matrix<T> (int width, int height):m_width(width), m_height(height){
            m_data.reserve(m_width);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                m_data.push_back(vector<T>());
                m_data[i].reserve(m_height);
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    m_data[i].push_back(T());
                }
            }
        }
        /**
         * @brief Constructor for matrix of size width x height with given default value.
         *
         * @param width Matrix width (columns)
         * @param height Matrix height (rows)
         * @param value Default value to fill the matrix with
         * @return Matrix of size width x height
         */
        template<class P>
        matrix<T> (int width, int height, P value):m_width(width), m_height(height){
            m_data.reserve(m_width);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                m_data.push_back(vector<T>());
                m_data[i].reserve(m_height);
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    m_data[i].push_back(value);
                }
            }
        }
        /**
         * @brief Constructor for matrix using 2D vector as input. Matrix size and values are taken from the vector.
         *
         * @param input_matrix 2D vector specifing the matrix
         * @return Matrix
         */
        matrix<T> (vector<vector< T > > input_matrix){
            m_width = input_matrix.size();
            m_height = input_matrix[0].size();

            m_data.reserve(m_width);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                m_data.push_back(vector<T>());
                m_data[i].reserve(m_height);
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    m_data[i].push_back(input_matrix[i][c]);
                }
            }
        }
        /**
         * @brief Constructor for matrix using 2D array as input. Matrix size and values are taken from the array.
         *
         * @param input_matrix 2D array specifing the matrix
         * @return Matrix
         */
        template<int W, int H, class M>
        matrix<T> (M (&input_matrix)[W][H]){
            m_width = W;
            m_height = H;

            m_data.reserve(m_width);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                m_data.push_back(vector<T>());
                m_data[i].reserve(m_height);
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    m_data[i].push_back(input_matrix[i][c]);
                }
            }
        }
        /**
         * @brief Destructor freeing memory used by the matrix.
         */
        ~matrix(){
            for ( unsigned int i=0 ; i < m_width; ++i ){
                m_data[i].clear();
            }
            m_data.clear();
        }

        /**
         * @brief Overload for the = operator
         *
         * @param input_matrix 2D vector specifing the matrix
         * @return Matrix
         */
        matrix<T>& operator= (const matrix<T> &mSource){
            //Check self assignment
            if (this == &mSource)
                return *this;

            if (m_width == mSource.m_width && m_height == mSource.m_height){
                for ( unsigned int i=0 ; i < m_width; ++i ){
                    for ( unsigned int c=0 ; c < m_height; ++c ){
                        m_data[i][c] = mSource.m_data[i][c];
                    }
                }
                //SHOWERROR("FFS THIS GOT RAN!\n");
            }else{
                m_width = mSource.m_width;
                m_height = mSource.m_height;
                m_data = mSource.m_data;
                //SHOWERROR("THIS RAN RAN!\n");
            }
            return *this;
        }

        /**
         * @brief Overload for the [] operator
         *
         * @param index Index for the row. Vector will take care about the column index.
         * @return Value at matrix position [row][column]
         */
        template<class P>
        vector<T>& operator[] (const P index){
            if (index<0 or index>m_width)
                //SHOWERROR("Index out of range! Requested index: %i in [%i,%i] matrix\n",index,m_width,m_height);
            return m_data[index];
        }

        /*******************************************************************************************/
        //<-----------------------------------------ADDITION---------------------------------------->
        /*******************************************************************************************/
        /**
         * @brief Overload for the += operator, allowing addition of default data types
         */
        template<class P>
        matrix<T>& operator+= (const P rhs){
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    m_data[i][c] += rhs;
                }
            }
            return *this;
        }

        /**
         * @brief Overload for the += operator, allowing addition of matrices
         */
        template<class M>
        matrix<T>& operator+= (const matrix<M> &rhs) {
            if (m_width!=rhs.m_width || m_height!=rhs.m_height){
                //SHOWERROR("Matrix size do not match for addition! Width: %i!=%i || Height: %i!=%i\n",m_width,rhs.m_width,m_height,rhs.m_height);
                return *this;
            }
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    m_data[i][c] += rhs.m_data[i][c];
                }
            }
            return *this;
        }

        /**
         * @brief Overload for the + operator, allowing addition of default data types
         */
        template<class P>
        matrix<T> operator+ (const P rhs) {
            matrix<T> new_matrix(m_width,m_height);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    new_matrix.m_data[i][c] = m_data[i][c] + rhs;
                }
            }
            return new_matrix;
        }

        /**
         * @brief Overload for the + operator, allowing addition of matrices
         */
        template<class M>
        matrix<T> operator+ (const matrix<M> &rhs){
            if (m_width!=rhs.m_width || m_height!=rhs.m_height){
                //SHOWERROR("Matrix size do not match for addition! Width: %i!=%i || Height: %i!=%i\n",m_width,rhs.m_width,m_height,rhs.m_height);
                return *this;
            }
            matrix<T> new_matrix(m_width,m_height);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    new_matrix.m_data[i][c] = m_data[i][c] + rhs.m_data[i][c];
                }
            }
            return new_matrix;
        }

        /*******************************************************************************************/
        //<----------------------------------------SUBTRACTION------------------------------------->
        /*******************************************************************************************/
        /**
         * @brief Overload for the -= operator, allowing subtraction of default data types
         */
        template<class P>
        matrix<T>& operator-= (const P rhs){
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    m_data[i][c] -= rhs;
                }
            }
            return *this;
        }

        /**
         * @brief Overload for the -= operator, allowing subtraction of matrices
         */
        template<class M>
        matrix<T>& operator-= (const matrix<M> &rhs) {
            if (m_width!=rhs.m_width || m_height!=rhs.m_height){
                //SHOWERROR("Matrix size do not match for subtraction! Width: %i!=%i || Height: %i!=%i\n",m_width,rhs.m_width,m_height,rhs.m_height);
                return *this;
            }
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    m_data[i][c] -= rhs.m_data[i][c];
                }
            }
            return *this;
        }

        /**
         * @brief Overload for the - operator, allowing subtraction of default data types
         */
        template<class P>
        matrix<T> operator- (const P rhs) {
            matrix<T> new_matrix(m_width, m_height);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    new_matrix.m_data[i][c] = m_data[i][c] - rhs;
                }
            }
            return new_matrix;
        }

        /**
         * @brief Overload for the - operator, allowing subtraction of matrices
         */
        template<class M>
        matrix<T> operator- (const matrix<M> &rhs){
            if (m_width!=rhs.m_width || m_height!=rhs.m_height){
                //SHOWERROR("Matrix size do not match for subtraction! Width: %i!=%i || Height: %i!=%i\n",m_width,rhs.m_width,m_height,rhs.m_height);
                return *this;
            }
            matrix<T> new_matrix(m_width, m_height);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    new_matrix.m_data[i][c] = m_data[i][c] - rhs.m_data[i][c];
                }
            }
            return new_matrix;
        }

        /*******************************************************************************************/
        //<---------------------------------------MULTIPLICATION------------------------------------>
        /*******************************************************************************************/
        /**
         * @brief Overload for the *= operator, allowing multiplication with default data types
         */
        template<class P>
        matrix<T>& operator*= (const P rhs){
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    m_data[i][c] *= rhs;
                }
            }
            return *this;
        }

        /**
         * @brief Overload for the *= operator, allowing multiplication of matrices
         */
        template<class M>
        matrix<T>& operator*= (const matrix<M> &rhs){
            if (m_height!=rhs.m_width){
                //SHOWERROR("Matrices must be m*n and n*p to multiply! Given matrices have sizes of %ix%i and %ix%i\n",m_width,m_height,rhs.m_width,rhs.m_height);
                return *this;
            }
            matrix<T> new_matrix(m_width,rhs.m_height);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < rhs.m_height; ++c ){
                    for ( unsigned int k=0 ; k < m_height; ++k ){
                        new_matrix.m_data[i][c] += m_data[i][k] * rhs.m_data[k][c];
                    }
                }
            }
            m_data = new_matrix.m_data;
            m_height = rhs.m_height;
            return *this;
        }

        /**
         * @brief Overload for the * operator, allowing multiplication with default data types
         */
        template<class P>
        matrix<T> operator* (const P rhs){
            matrix<T> new_matrix(m_width, m_height);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    new_matrix.m_data[i][c] = m_data[i][c] * rhs;
                }
            }
            return new_matrix;
        }

        /**
         * @brief Overload for the * operator, allowing multiplication of matrices
         */
        template<class M>
        matrix<T> operator* (const matrix<M> rhs){
            if (m_height!=rhs.m_width){
                //SHOWERROR("Matrices must be m*n and n*p to multiply! Given matrices have sizes of %ix%i and %ix%i\n",m_width,m_height,rhs.m_width,rhs.m_height);
                return *this;
            }
            matrix<T> new_matrix(m_width,rhs.m_height);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < rhs.m_height; ++c ){
                    for ( unsigned int k=0 ; k < m_height; ++k ){
                        new_matrix.m_data[i][c] += m_data[i][k] * rhs.m_data[k][c];
                    }
                }
            }
            return new_matrix;
        }

        /*******************************************************************************************/
        //<---------------------------------------DIVISION------------------------------------------>
        /*******************************************************************************************/
        /**
         * @brief Overload for the /= operator, allowing division with default data types
         */
        template<class P>
        matrix<T>& operator/= (P rhs){
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    m_data[i][c] /= rhs;
                }
            }
            return *this;
        }

        /**
         * @brief Overload for the /= operator, allowing division of matrices
         */
        template<class M>
        matrix<T>& operator/= (matrix<M> &rhs){
            matrix<T> inverted_matrix = rhs.inverse();
            matrix<T> new_matrix(m_width,inverted_matrix.m_height);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < inverted_matrix.m_height; ++c ){
                    for ( unsigned int k=0 ; k < m_height; ++k ){
                        new_matrix.m_data[i][c] += m_data[i][k] * inverted_matrix.m_data[k][c];
                    }
                }
            }
            m_data = new_matrix.m_data;
            m_height = new_matrix.m_height;
            return *this;
        }

        /**
         * @brief Overload for the / operator, allowing division with default data types
         */
        template<class P>
        matrix<T> operator/ (P rhs){
            matrix<T> new_matrix(m_width,m_height);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    new_matrix.m_data[i][c] = m_data[i][c] / rhs;
                }
            }
            return new_matrix;
        }

        /**
         * @brief Overload for the / operator, allowing division of matrices
         */
        template<class M>
        matrix<T> operator/ (matrix<M> &rhs){
            matrix<T> inverted_matrix = rhs.inverse();
            matrix<T> new_matrix(m_width,inverted_matrix.m_height);
            for ( unsigned int i=0 ; i < m_width; ++i ){
                for ( unsigned int c=0 ; c < inverted_matrix.m_height; ++c ){
                    for ( unsigned int k=0 ; k < m_height; ++k ){
                        new_matrix.m_data[i][c] += m_data[i][k] * inverted_matrix.m_data[k][c];
                    }
                }
            }
            return new_matrix;
        }

        /*******************************************************************************************/
        //<----------------------------------------MEMBERS------------------------------------------>
        /*******************************************************************************************/
        /**
         * @brief Make the matrix into an identity matrix where there are 1's on the diagonal and 0's everywhere else
         */
        matrix<T> identity(){
            matrix<T> new_matrix(m_width,m_height);
            for (unsigned int i=0; i<m_width; ++i){
                for (unsigned int c=0; c<m_height; ++c){
                    new_matrix.m_data[i][c]=(i==c);
                }
            }
            return new_matrix;
        }

        /**
         * @brief Fill the matrix with a value
         */
        template<class P>
        void fill(P value){
            for (unsigned int i=0; i<m_width; ++i){
                for (unsigned int c=0; c<m_height; ++c){
                    m_data[i][c]=value;
                }
            }
        }

        /**
         * @brief Transpose matrix
         *
         * @return Returns a transposed matrix
         */
        matrix<T> transpose(){
            matrix<T> new_matrix(m_height,m_width);
            for (unsigned int i=0; i<m_width; ++i){
                for (unsigned int c=0; c<m_height; ++c){
                    new_matrix.m_data[c][i] = m_data[i][c];
                }
            }
            return new_matrix;
        }

        /**
         * @brief Calculate Cholesky triangulation for the matrix. Used mostly in inverse
         *
         * @return Returns a Cholesky triangulation of the matrix
         */
        matrix<T> Cholesky(double ztol=1.0e-5){
            matrix<T> res(m_width, m_width);
            for (unsigned int i=0; i<m_width; ++i){
                T S = T(); //Init to default value. Usually 0
                for (unsigned int k=0; k<i; ++k)
                    S += res.m_data[k][i]*res.m_data[k][i];
                T d = m_data[i][i] - S;
                if (abs(d) < ztol){
                    res.m_data[i][i] = T();
                }else{
                    if (d < T())
                    {
                        //SHOWERROR("Matrix not positive-definite (singularity) \n");
                    }
                    res.m_data[i][i] = sqrt(d);
                }
                for (unsigned int j=i+1; j<m_width; ++j){
                    S = T();
                    for (unsigned int k=0; k<m_width; ++k){
                        S += res.m_data[k][i] * res.m_data[k][j];
                        if (abs(S)<ztol){
                            S = T();
                        }
                        res.m_data[i][j] = (m_data[i][j] - S) / res.m_data[i][i];
                    }
                }
            }
            return res;
        }

        /**
         * @brief Calculate Cholesky inverse matrix
         *
         * @return Returns a Cholesky inverse matrix
         */
        matrix<T> CholeskyInverse(double ztol=1.0e-5){
            matrix<T> res(m_width,m_width);
            for (int i=m_width-1; i>=0; --i){
                T tjj = m_data[i][i];
                T S = T();
                for (unsigned int k=i+1; k<m_width; ++k){
                    S += m_data[i][k] * res.m_data[i][k];
                }
                res.m_data[i][i] = 1.0 / (tjj*tjj) - S / (tjj);
                for (int c=i-1; c>=0; --c){
                    S = T();
                    for (unsigned int k=c+1; k<m_width; ++k){
                        S += m_data[c][k]*res.m_data[k][i];
                    }
                    res.m_data[i][c] = res.m_data[c][i] = -S/m_data[c][c];
                }
            }
            return res;
        }

        /**
         * @brief Inverts the matrix
         *
         * @return Returns an inverted matrix
         */
        matrix<T> inverse(){
            if (m_width!=m_height){
                //SHOWERROR("Inverse can only be calculated for a square matrix! %i!=%i \n",m_width,m_height);
                return *this;
            }
            if (m_width == 2 && m_height == 2){ //Special case for 2x2 matrix for speed
                matrix<T> tmp(2,2);
                tmp.m_data[0][0] = m_data[1][1];
                tmp.m_data[1][0] = -m_data[1][0];
                tmp.m_data[0][1] = -m_data[0][1];
                tmp.m_data[1][1] = m_data[0][0];
                tmp *= 1.0/(m_data[0][0]*m_data[1][1]-m_data[1][0]*m_data[0][1]);
                return tmp;
            }else{
                matrix<T> tmp = this->Cholesky();
                tmp = tmp.CholeskyInverse();
                return tmp;
            }
        }

        /*******************************************************************************************/
        //<----------------------------------------OUTPUT------------------------------------------->
        /*******************************************************************************************/
        /**
         * @brief Prints out a pretty matrix
         */
        string print(){
            string str = "";
            for ( unsigned int i=0 ; i < m_width; ++i ){
                str += "[";
                for ( unsigned int c=0 ; c < m_height; ++c ){
                    str += m_data[i][c];
                    if (c<m_height-1) str += ", ";
                }
                str += "]\n";
            }
            return str;
        }
};

template <class T>
class quaternion
{
     public:
        T m_w, m_x, m_y, m_z;
        /**
         * @brief Constructor for default (unrotated) quaternion
         */
        quaternion<T> ():m_w(1),m_x(0),m_y(0),m_z(0){ };

        /**
         * @brief Constructor for a rotated quaternion with an axis unit vector (x,y,z) and angle (angle)
         */
        quaternion<T> (T x, T y, T z, T angle){
            m_w = cos( angle/2.0 );
            m_x = x * sin( angle/2.0 );
            m_y = y * sin( angle/2.0 );
            m_z = z * sin( angle/2.0 );
        }

        /**
         * @brief Normalize quaternion. So turn it back to unit quaternion (magnitude == 1)
         */
        void normalize(){
            T magnitude = sqrt(m_w*m_w + m_x*m_x + m_y*m_y + m_z*m_z);
            m_w /= magnitude;
            m_x /= magnitude;
            m_y /= magnitude;
            m_z /= magnitude;
        }

        /**
         * @brief Overload for the * operator, allowing multiplication with other quaternions
         */
        template<class M>
        quaternion<T> operator* (const quaternion<M> rhs){
            quaternion<T> new_quaternion;
            new_quaternion.m_w = (m_w * rhs.m_w - m_x * rhs.m_x - m_y * rhs.m_y - m_z * rhs.m_z);
            new_quaternion.m_x = (m_w * rhs.m_x + m_x * rhs.m_w + m_y * rhs.m_z - m_z * rhs.m_y);
            new_quaternion.m_y = (m_w * rhs.m_y - m_x * rhs.m_z + m_y * rhs.m_w + m_z * rhs.m_x);
            new_quaternion.m_z = (m_w * rhs.m_z + m_x * rhs.m_y - m_y * rhs.m_x + m_z * rhs.m_w);
            return new_quaternion;
        }

        /**
         * @brief Overload for the *= operator, allowing multiplication with other quaternions
         */
        template<class M>
        quaternion<T>& operator*= (const quaternion<M> &rhs){
            quaternion<T> new_quaternion;
            new_quaternion.m_w = (m_w * rhs.m_w - m_x * rhs.m_x - m_y * rhs.m_y - m_z * rhs.m_z);
            new_quaternion.m_x = (m_w * rhs.m_x + m_x * rhs.m_w + m_y * rhs.m_z - m_z * rhs.m_y);
            new_quaternion.m_y = (m_w * rhs.m_y - m_x * rhs.m_z + m_y * rhs.m_w + m_z * rhs.m_x);
            new_quaternion.m_z = (m_w * rhs.m_z + m_x * rhs.m_y - m_y * rhs.m_x + m_z * rhs.m_w);
            m_w = new_quaternion.m_w;
            m_x = new_quaternion.m_x;
            m_y = new_quaternion.m_y;
            m_z = new_quaternion.m_z;
            return *this;
        }

        /**
         * @brief Set quaternion with an axis unit vector (x,y,z) and angle (angle)
         */
        void set(T x, T y, T z, T angle){
            m_w = cos( angle/2.0 );
            m_x = x * sin( angle/2.0 );
            m_y = y * sin( angle/2.0 );
            m_z = z * sin( angle/2.0 );
        }

        /**
         * @brief Create a rotation matrix from the quaternion
         * @todo FIX TEMPLATES, DOES THIS REALLY HAVE TO BE A DOUBLE?
         */
        matrix<double> rotationMatrix(){
            double R[][4] = {{1.0-2.0*m_y*m_y-2.0*m_z*m_z, 2.0*m_x*m_y-2.0*m_w*m_z, 2.0*m_x*m_z+2.0*m_w*m_y, 0.0},
                        {2.0*m_x*m_y+2.0*m_w*m_z, 1-2.0*m_x*m_x-2.0*m_z*m_z, 2.0*m_y*m_z+2.0*m_w*m_x, 0.0},
                        {2.0*m_x*m_z-2.0*m_w*m_y, 2.0*m_y*m_z-2.0*m_w*m_x, 1-2.0*m_x*m_x-2.0*m_y*m_y, 0.0},
                        {0.0, 0.0, 0.0, 1.0}};
            matrix<double> R_matrix(R);
            return R_matrix;
        }
};

}

#endif
