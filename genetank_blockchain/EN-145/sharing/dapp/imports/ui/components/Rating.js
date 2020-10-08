import React, { Component } from 'react';

export class Rating extends Component {
  constructor(props) {
    super(props);
  }

  render() {
    return (
      <span>
        <span className="score mr-1">
          <i className={this.props.score >= 1 ? "fas fa-star" : "far fa-star"}></i>
          <i className={this.props.score >= 2 ? "fas fa-star" : "far fa-star"}></i>
          <i className={this.props.score >= 3 ? "fas fa-star" : "far fa-star"}></i>
          <i className={this.props.score >= 4 ? "fas fa-star" : "far fa-star"}></i>
          <i className={this.props.score >= 5 ? "fas fa-star" : "far fa-star"}></i>
        </span>
        <span className="mr-2">{this.props.ratings}</span>
        <i className="fas fa-comment-alt mr-1"></i>
        <span className="mr-2">{this.props.comments}</span>
        <i className="fas fa-eye mr-1"></i>
        <span>{this.props.views}</span>
      </span>
    )
  }
}
