using System;

namespace Grassman
{
    public abstract class AbstractTensorMatrix : AbstractMatrix
    {
        public int K { get; protected internal set; } //Exterior algebra's subspace dimension
        protected internal int line, column; //Current matrix position (see Loop)
        protected internal int b; //0 - loop by line, 1 - loop by column (see Loop)
        protected internal Matrix M; //Initial matrix

        //Required for checking the validation of the constructor arguments
        protected static readonly Func<Matrix, int, bool> constructorCheck = (M, k) => M == null || M.W != M.H || k <= 0;

    } //Base class for our tensor(generalized exterior) algebra matrix
}
