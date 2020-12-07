package org.krieger.beerbot.data;

/**
 * roboter position data
 * 
 * @author tkrieger
 *
 */
public class Position {
	
	/**
	 * 2d x position 
	 */
	private double x;
	
	/**
	 * 2d y position 
	 */
	private double y;
	
	/**
	 * current rotation angle
	 */
	private double theta;
	
	/**
	 * default position 
	 * 
	 * @param x
	 * @param y
	 * @param theta
	 */
	public Position(double x,double y,double theta) {
		this.x     = x;
		this.y     = y;
		this.theta = theta;
	}

	public double getX() {
		return x;
	}

	public double getY() {
		return y;
	}

	public double getTheta() {
		return theta;
	}

}
