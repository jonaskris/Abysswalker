package math;

public class Vector {

    private float[] vector;

    public Vector(float[] values){
        vector = new float[values.length];
        for(int i = 0; i < values.length; i++){
            vector[i] = values[i];
        }
    }

    public Vector(int length){
        vector = new float[length];
    }

    public void set(int i, float value){
        vector[i] = value;
    }

    public float get(int i){
        return vector[i];
    }

    public Vector copy(){
        return new Vector(vector);
    }

    public int size(){
        return vector.length;
    }

    public String toString(){
        StringBuilder returnString = new StringBuilder();
        returnString.append("[");
        for(int i = 0; i < vector.length; i++){
            returnString.append((i == vector.length - 1)?(vector[i] + "]\n"):(vector[i] + ", "));
        }
        return returnString.toString();
    }

    @Override
    public boolean equals(Object obj) {
        if(!(obj instanceof Vector)){
            return false;
        }

        if(((Vector) obj).size() != size()){
            return false;
        }

        for(int i = 0; i < size(); i++){
            if(get(i) != ((Vector) obj).get(i)){
                return false;
            }
        }

        return true;
    }

    public Vector add(Vector values){
        if(values.size() != size()){
            throw new IllegalArgumentException("Values must be the same length as the vector!");
        }
        float[] newVectorValues = new float[size()];
        for(int i = 0; i < newVectorValues.length; i++){
            newVectorValues[i] = vector[i] + values.get(i);
        }
        return new Vector(newVectorValues);
    }

    public Vector add(float value){
        float[] newVectorValues = new float[size()];
        for(int i = 0; i < newVectorValues.length; i++){
            newVectorValues[i] = vector[i] + value;
        }
        return new Vector(newVectorValues);
    }

    public Vector multiply(float value){
        float[] newVectorValues = new float[size()];

        for(int i = 0; i < newVectorValues.length; i++){
            newVectorValues[i] = vector[i] * value;
        }

        return new Vector(newVectorValues);
    }

    public float dot(Vector that){
        if(this.size() != that.size()){
            throw new IllegalArgumentException("Two vectors must be the same length to take their dot product!");
        }

        float returnValue = 0;

        for(int i = 0; i < size(); i++){
            returnValue += this.get(i) * that.get(i);
        }

        return returnValue;
    }

    public double magnitude(){
        float foo = 0;

        for(int i = 0; i < vector.length; i++){
            foo += vector[i]*vector[i];
        }

        return Math.sqrt(foo);
    }

    public static void main(String[] args) {
        Vector a = new Vector(4);

        a.set(0, 1.0f);
        a.set(1, 2.0f);
        a.set(2, 3.0f);

    }

}