import { Request } from 'express';

export interface IUserRequest extends Request {
  user: any;
}

export interface IUserProfile {
  id: string;
  email: string;
  username: string;
  gender: string;
  age: string;
  ethnicity: string;
  location_city: string;
  location_province: string;
  location_country: string;
  location_zip: string;
  notifications_email: string;
}

export interface IQuestion {
  id: number;
  question: string;
  answer: string[];
  type: string;
}

export interface ICampaignBasic {
  name: string;
  code: string;
  image: string;
  description: string;
}

export interface ICampaign {
  name: string;
  code: string;
  disease_id: string;
  image: string;
  description: string;
  start_time: Date;
  end_time: Date;
  num_pledges_data: number;
  num_pledges_money: number;
  total_money: number;
  goal_data: number;
  min_money_donation: number;
  gwas_report_price: number;
  prediction_price: number;
}

export interface IBodyMap {
  gender: string;
  data: [{
    part: string;
    name: string;
    disease: [{
      name: string;
      level: string;
    }];
  }];
}
