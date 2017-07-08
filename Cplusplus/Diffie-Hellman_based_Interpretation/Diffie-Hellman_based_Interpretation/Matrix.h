#pragma once
#include <ctime>
#include <stdlib.h>
#include <vector>
#include <list>

namespace Grassman {
	class AbstractMatrix {

	public:
		AbstractMatrix() {
			SetData({});
		}
		AbstractMatrix(unsigned height, unsigned width) {
			resize(height, width);
		}

		unsigned getH() {
			return h;
		}
		unsigned getW() {
			return w;
		}

		void setW(unsigned newW) {
			w = newW;
			data.resize(w);
		}
		void setH(unsigned newH) {
			h = newH;
			for (vector<int> line : data) {
				line.resize(h);
			}
		}
		void resize(unsigned newH, unsigned newW) { //stelat' po normalnomu
			setW(newW);
			setH(newH);
		}

		~AbstractMatrix() {
			data.clear();
		}
		vector<int> operator [](unsigned i) { return data[i]; } // Allows use M[i][j] syntax
	protected:
		void SetData(vector<vector<int>> newData) {
			data.clear();
			unsigned size = newData.size();
			setW(size);
			if (size) {
				data = newData;
				setH(data[0].size()); // expected that all columns aligned
			}
			else
				setH(0);
		}
	
	private:
		unsigned h;
		unsigned w;
		vector<vector<int>> data;
	}; // Common matrix class

	class Matrix : AbstractMatrix {
		Matrix() : AbstractMatrix () {}
		Matrix(unsigned height, unsigned width, int minvalue, int maxvalue) : AbstractMatrix(height, width) {
			for (unsigned i = 0; i < width; i++) {
				for (unsigned j = 0; j < width; j++){
					(*this)[i][j] = minvalue + rand() % maxvalue;
				}
			}
		}

		Matrix(vector<vector<int>> Data) : AbstractMatrix() {
			SetData(Data);
		}
		
	}; //General matrix class

	class GrassmanMatrix : AbstractMatrix {

		/// <summary>
		/// Creates an exterior algebra subspace matrix based on M, which has the dimension of k
		/// </summary>
		/// <param name="M"></param>
		/// <param name="k"></param>
		GrassmanMatrix(Matrix M, unsigned k) : AbstractMatrix(){

			// source matrix shouldn't be less than k*k
			if (k < M.getH < M.getW ? M.getH : M.getW) {
				this->SetData({});
				return;
			}

			this->M = M;
			K = k;

			this->setW(C(M.getW, k));
			this->setH(M.getH == M.getW ? this->getW : C(M.getH, k));

			vector<vector<int>> data(this->getW, vector<int>(this->getH));
			LineIndex = *(new vector<unsigned>);
			Loop(k);
			column = 0;
			line = 0;
			LineIndex.clear();
			ColumnIndex.clear();
			LoopState = true;
		}
		GrassmanMatrix() : AbstractMatrix() {}
	private:
		unsigned K;
		unsigned column = 0;
		unsigned line = 0;
		vector<unsigned> ColumnIndex, LineIndex;
		Matrix M;
		bool LoopState = true;

		/// <summary>
		/// Returns the number of combinations of k elements from n-elements set
		/// </summary>
		/// <param name="n"></param>
		/// <param name="k"></param>
		/// <returns></returns>
		unsigned C(unsigned n, unsigned k) { 
			if (n < k)
				return 0;
			unsigned ret = 1;
			for (unsigned i = 1; i <= k; ++i)
				ret = ret * (n - k + i) / i;
			return ret;
		}
		/// <summary>
		/// search of all combination for columns or lines \
			passing through n nested loops, then call Minor for this\
			combination
		/// </summary>
		/// <param name="n"></param>
		void Loop(unsigned n) {
			if (n != 0) {
				//defining current index list
				vector<unsigned> Index = LoopState ? LineIndex : ColumnIndex;
				unsigned i = Index.empty ? 0 : Index.back() + 1;
				
				// recursive combinations search
				Index.push_back(i);
				for (; i <= this->getW - n; i++) {
					Index.back()++;
					Loop(n - 1);
				}
				Index.pop_back();
			} 
			else {
				if (LoopState) {
					LoopState = false;
					Loop(K);
					LoopState = true;
				} 
				else {
					(*this)[line][column] = Minor(K);
					if (line == this->getH) {
						line++;
						column = 0;
					}
					else
						column++;
				}
			}
		}
		/// <summary>
		/// j-th minor, whose lines and columns are defined by LineIndex and ColumnIndex
		/// </summary>
		/// <param name="j"></param>
		/// <returns></returns>
		int Minor(unsigned j) {
			if (j == 1) {
				return M[LineIndex.back()][ColumnIndex.back()];
			}
			else {
				int sign = 1;
				int determinant = 1;
				for (unsigned i = 0; i < ColumnIndex.size(); i++) {
					unsigned temporaryIndex = ColumnIndex[i];
					ColumnIndex.erase(ColumnIndex.begin() + i);
					determinant += sign * M[LineIndex[K - j]][temporaryIndex] * Minor(j - 1);
					ColumnIndex.insert(ColumnIndex.begin() + i, temporaryIndex);
					sign *= -1;
				}
				return determinant;
			}
		}
	};
}

//
//namespace Grassman
//{
//	class Vector {
//		operator int*() { return (int*)this; }
//	};
//
//	class AbstractMatrix
//	{/*
//	//public uint W{ get; protected set; } //Width
//	//public uint H{ get; protected set; } //Height
//	public: unsigned getH() const { return h; }
//	public: void setH(unsigned newH) { h = newH; }
//
//		protected int[, ] array; //The matrix itself
//								 /*
//								 public AbstractMatrix(uint w, uint h)
//								 {
//								 array = new int[w, h];
//								 W = w;
//								 H = h;
//								 }
//								 *\/
//
//		public int this[int i, int j] //Allows to address A.array[i, j] as A[i,j]
//		{
//			get{ return array[i, j]; }
//		set{ array[i, j] = value; }
//		}
//		*/
//	public: 
//		operator Vector*() { return data; }
//	private:
//		Vector data[];
//	}; //Common matrix class
//
//
//	class Matrix : AbstractMatrix
//	{
//		// Creates a w*h matrix, whose elements are random and belong to the [-minvalue, maxvalue] line segment
//	public: 
//		Matrix(unsigned w, unsigned h, int minvalue, int maxvalue)//потом будет по модулю p
//		{
//			unsigned W = w;
//			unsigned H = h;
//			//array = new int[w, h];
//			vector<vector<int>> array();
//			for (int i = 0; i < w; ++i)
//				for (int j = 0; j < h; ++j)
//					this[i][j] = rand() % (maxvalue - minvalue + 1) - minvalue;
//		}
//
//		// Creates a w*h matrix, based on given 2-dimensional array
//		public Matrix(uint w, uint h, int[, ] array)
//		{
//			W = w;
//			H = h;
//			this.array = array;
//		}
//		/*
//		public Matrix(uint w, uint h)
//		{
//		array = new int[w, h];
//		W = w;
//		H = h;
//		}
//		*/
//
//	} //General matrix class
//
//	class GrassmanMatrix : AbstractMatrix
//	{
//		public uint K{ get; private set; } //Exterior algebra's subspace dimension
//		int line = 0, column = 0; //Current matrix position (see Loop)
//		List<uint> lineIndex, columnIndex; //Lists of ordered columns' and rows' numbers (see Loop, Minor)
//		bool loopState = true; //True stands for a loop by line, False - by column (see Loop)
//		Matrix M; //Initial
//
//				  /// <summary>
//				  /// Creates an exterior algebra subspace matrix based on M, which has the dimension of k
//				  /// </summary>
//				  /// <param name="M"></param>
//				  /// <param name="k"></param>
//		public GrassmanMatrix(Matrix M, uint k)
//		{
//			this.M = M;
//			K = k;
//			W = C(M.W, k);
//			H = M.W == M.H ? W : C(M.H, k);
//			array = new int[W, H];
//			lineIndex = new List<uint>();
//			Loop(k);
//			column = 0;
//			line = 0;
//			lineIndex = null;
//			columnIndex = null;
//			loopState = true;
//		}
//
//		/// <summary>
//		/// Returns the number of k-combinations from n elements
//		/// </summary>
//		/// <param name="n"></param>
//		/// <param name="k"></param>
//		/// <returns></returns>
//		uint C(uint n, uint k)//сделать
//		{
//			if (n < k)
//				return 0;
//			uint ret = 1;
//			for (uint i = 1; i <= k; ++i)
//				ret = ret * (n - k + i) / i;
//			return ret;
//		}
//
//
//		/// <summary>
//		/// n nested loops
//		/// </summary>
//		/// <param name="n"></param>
//		void Loop(uint n)
//		{
//			if (n != 0)
//			{
//				List<uint> index = loopState ? lineIndex : columnIndex;
//				uint i = index.Any() ? index.Last() + 1 : 0;
//				int j = index.Count;
//				index.Add(i);
//				for (; i <= W - n; ++i)
//				{
//					Loop(n - 1);
//					++index[j];
//				}
//				index.Remove(index.Last());
//			}
//			else
//			{
//				if (loopState)
//				{
//					loopState = false;
//					columnIndex = new List<uint>();//index == wlist
//					Loop(K);
//					loopState = true;
//				}
//				else
//					this[line, column] = Minor(K);
//				if (loopState)
//				{
//					++line;
//					column = 0;
//				}
//
//				else
//					++column;
//			}
//		}
//
//		/// <summary>
//		/// j-th minor, whose lines and columns are defined by wlist and hlist
//		/// </summary>
//		/// <param name="j"></param>
//		/// <returns></returns>
//		int Minor(uint j)
//		{
//			if (j == 1)
//				return M[(int)lineIndex.Last(), (int)columnIndex.Last()];
//			int sgn = 1, ret = 0, count = columnIndex.Count;
//			for (int i = 0; i < count; ++i)
//			{
//				uint t = columnIndex[i];
//				columnIndex.RemoveAt(i);
//				ret += sgn * M[(int)lineIndex[(int)(K - j)], (int)t] * Minor(j - 1);
//				columnIndex.Insert(i, t);
//				sgn *= -1;
//			}
//			return ret;
//		}
//	} //Exterior algebra matrix
//
//	class Program
//	{
//		static void Main(string[] args)
//		{
//			int[, ] arr = { { 5, -2, 0 },{ 1, 3, 2 },{ 4, 1, 7 } };
//			Matrix A = new Matrix(3, 3, arr);
//			GrassmanMatrix G = new GrassmanMatrix(A, 2);
//			Console.ReadKey();
//		}
//	}
//}